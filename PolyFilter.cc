#include "PolyFilter.hh"

// size is the number of particles in each dimension
void PolyFilter::init ( int _n_samples, int _deg) {

  n_samples = _n_samples;
  deg = _deg;

  assert(deg == 2);
  new_dim = 1;
  new_dim += get_dim();
  new_dim += get_dim() * (get_dim()+1) / 2;

  motion_offset.resize(get_dim(),0);

  train_x.resize(new_dim, n_samples);
  train_y.resize(1, n_samples);

  coeff.resize(1, new_dim);



  cdf.resize(get_total_size(), 1./get_total_size());
   // CDF I/O
  name = "poly-filter-" + std::to_string (n_samples) + ".csv";
  string path = get_test_path() + "cdf-" + name;
  cdf_fid.open(path);
  path = get_test_path() + "pdf-" + name;
  pdf_fid.open(path);
}

void PolyFilter::process () {
  
  clock_start ();

  static bool first_training = true;
  static std::uniform_real_distribution<double> unif_d(0, 1);
  static std::uniform_int_distribution<int> unif_i(0, get_total_size()-1);

  // Motion update
  state_t state = state_t(get_dim(),0);
  state = motion_update(state);
  for (int i = 0; i < get_dim(); ++i) {
    motion_offset[i] += state[i];
  }


  // TEMP Use all data points
  n_samples = get_total_size();
  train_x.resize(new_dim, n_samples);
  train_y.resize(1, n_samples);

  // for (int i_sample = 0; i_sample < n_samples; ++i_sample) {
  //   int rand = i_sample;

  //   state_t state = index_to_state(id_to_index(rand));
  //   for (int i = 0; i < get_dim(); ++i) {
  //     state[i] -= motion_offset[i];
  //   }

  //   auto new_state = poly_state (state);
  //   for (int i = 0; i < new_dim; ++i) {
  //     train_x(i,i_sample) = new_state[i];
  //   }

  //   train_y(0,i_sample) = sensor_update (state);
  // }
  // End TEMP

  for (int i_sample = 0; i_sample < n_samples; ++i_sample) {
  	int rand;

  	// Purely random // TODO NOTE.  only using this
  	if (i_sample < int(0*n_samples)) {
  		rand = unif_i(generator);
  	}
  	// Using previous CDF
  	else {
  		rand = std::lower_bound(cdf.begin(), cdf.end(),
  			unif_d(generator)) - cdf.begin();
  		if (rand < 0 || rand >= get_total_size()) {
  			cout << "Problematic random number!!! " << rand << "\n";
  			rand = unif_i(generator);
  		}
  	}

  	state_t state = index_to_state(id_to_index(rand));
  	for (int i = 0; i < get_dim(); ++i) {
  		state[i] -= motion_offset[i];
  	}

    auto new_state = poly_state (state);
    for (int i = 0; i < new_dim; ++i) {
      train_x(i,i_sample) = new_state[i];
    }

  	train_y(0,i_sample) = sensor_update (state);
  }

  // if (first_training) {
  // 	first_training = false;
  // } else {
  // 	Matrix pred = coeff * train_x;
  // 	for (int i_sample = 0; i_sample < n_samples; ++i_sample) {
  // 	  train_y(0,i_sample) *= pred(0,i_sample);
  // 	}
  // }

  // // TEMP
  // cout << "train_x\n" << train_x ;
  // cout << "\n\n\ntrain_y\n" << train_y ;

  // Normalize train_y
  normalize(train_y);


  coeff = ((train_x * train_x.transpose()).ldlt().solve(train_x * train_y.transpose())).transpose();

  clock_stop ();  
}

void PolyFilter::store_cdf () {
  
  std::fill(cdf.begin(), cdf.end(), 0);

  test_x.resize(new_dim, get_total_size());

  for (int i = 0; i < get_total_size(); ++i) {
    state_t state = index_to_state(id_to_index(i));
    for (int j = 0; j < get_dim(); ++j) {
      state[j] -= motion_offset[j];
    }

    auto new_state = poly_state (state);

    test_x(0,i) = 1;
    for (int j = 0; j < new_dim; j++) {
      test_x(j,i) = new_state[j];
    }
  }

  test_y = coeff * test_x;

  normalize (test_y);

  // Construct approximate pdf. here pdf is called cdf.
  for (int i = 0; i < get_total_size(); ++i) {
  	cdf[i] = test_y(0,i);
  }

  io_store_pdf (cdf);
  compute_mean_state (cdf);
  pdf_to_cdf (cdf);
  io_store_cdf (cdf);
  io_store_error (cdf);
}

void PolyFilter::destroy () {
  pdf_fid.close();
  cdf_fid.close();
}

vector<double> PolyFilter::poly_state (const state_t& state) {
  
  vector<double> new_state(new_dim,0);

  // 0 deg
  int shift = 0;
  new_state[shift++] = 1;
  
  // 1 deg
  for (int i = 0; i < get_dim(); ++i) {
    new_state[shift++] = state[i];
  }

  // 2 deg
  shift = 1 + get_dim();
  for (int i = 0; i < get_dim(); ++i) {
    for (int j = 0; j <= i; ++j) {
      new_state[shift++] = state[i]*state[j];
    }
  }

  return new_state;

}

void PolyFilter::normalize (PolyFilter::Matrix &y) {

  double min_y = 0;
  for (int i = 0; i < y.cols(); ++i) {
    min_y = std::min (y(0,i), min_y);
  }

  double sum_y = 0;
  for (int i = 0; i < y.cols(); ++i) {
    y(0,i) -= min_y;
    sum_y += y(0,i);
  }

  for (int i = 0; i < y.cols(); ++i) {
    y(0,i) /= sum_y;
  }
}