#include "filter.hh"
#include "NeuralNetwork.hh"
#include <algorithm>

using namespace MiniDNN;

// size is the number of particles in each dimension
void NeuralNetwork::init ( int _nn_iter, int _nn_samples,
  vector<int> _layer_sizes, double l_rate = 0.01) {

  nn_iter = _nn_iter;
  nn_samples = _nn_samples;
  layer_sizes = _layer_sizes;

  motion_offset.resize(get_dim(),0);

  train_x.resize(get_dim()+1, nn_samples);
  train_y.resize(1, nn_samples);
  for (int i = 0; i < nn_samples; ++i) train_x(0,i) = 1;

  // NN build.
  // Setting hidden layers
  for (int i = 0; i <= layer_sizes.size(); ++i) {
    int left = (i > 0 ? layer_sizes[i-1] : get_dim()+1);

    Layer* layer;
    if (i == layer_sizes.size()) {
      layer = new FullyConnected<Sigmoid>(left, 1);
    } else {
      layer = new FullyConnected<ReLU>(left, layer_sizes[i]);
    }

    layers.push_back(layer);
    net.add_layer(layer);
  }

  // Last output layer
  net.set_output(new RegressionMSE());

  // Learning rate.
  opt.m_lrate = l_rate;

  // Loss print function.
  callback.m_nepoch = 100;
  net.set_callback(callback);

  // Initialize parameters with N(0, 0.01^2) using random seed 123
  net.init(0, 0.01, 123);


  cdf.resize(get_total_size(), 1./get_total_size());
  for (int i = 1; i < get_total_size(); ++i) cdf[i] += cdf[i-1];
   // CDF I/O
  name = "neural-network-" + std::to_string (nn_samples) + ".csv";
  string path = get_test_path() + "cdf-" + name;
  cdf_fid.open(path);
  path = get_test_path() + "pdf-" + name;
  pdf_fid.open(path);

  // NoNAO
  test_x.resize(get_dim()+1, get_total_size());
  test_y.resize(1, get_total_size());

  for (int i; i < get_total_size(); ++i) {
    state_t state = index_to_state(id_to_index(i));
    test_x(0,i) = 1;
    for (int j = 0; j < get_dim(); j++) {
      test_x(j+1,i) = state[j];
    }
  }
}

void NeuralNetwork::process () {
  
  clock_start ();

  static bool first_training = true;
  static std::uniform_real_distribution<double> unif_d(0, 1);
  static std::uniform_int_distribution<int> unif_i(0, get_total_size()-1);

  // Motion update
  state_t state = state_t(get_dim(),0);
  state = motion_update(state);
  for (int i = 0; i < get_dim(); ++i) {
  	motion_offset[i] += state[i];
  	cout << motion_offset[i];
  }

  for (int i_sample = 0; i_sample < nn_samples; ++i_sample) {
  	int rand;

  	// Purely random // TODO NOTE.  only using this
  	if (i_sample < int(2*nn_samples)) {
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
  		train_x(i+1,i_sample) = state[i] - motion_offset[i];
  	}

  	train_y(0,i_sample) = sensor_update (state) * get_total_size();
  }

  if (first_training) {
  	first_training = false;
  } else {
  	Matrix pred = net.predict(train_x);
  	for (int i_sample = 0; i_sample < nn_samples; ++i_sample) {
	  train_y(0,i_sample) *= pred(0,i_sample);
	}
  }

  cout << "train_x\n" << train_x ;
  cout << "train_y\n" << train_y ;

  // Normalize train_y
  // double sum_y = 0;
  // for (int i_sample = 0; i_sample < nn_samples; ++i_sample) {
  // 	sum_y += train_y(0,i_sample);
  // }
  // for (int i_sample = 0; i_sample < nn_samples; ++i_sample) {
  // 	train_y(0,i_sample) *= (nn_samples/sum_y);
  // }


  // Fit the model with a batch size of 100, running 10 epochs with random seed 123
  net.fit(opt, train_x, train_y, nn_samples, nn_iter, 123);

  clock_stop ();  
}

void NeuralNetwork::store_cdf () {
  
  std::fill(cdf.begin(), cdf.end(), 0);

  // Obtain prediction -- each column is an observation
  test_y = net.predict(test_x);

  // Construct approximate pdf. here pdf is called cdf.
  for (int i = 0; i < get_total_size(); ++i) {
  	cdf[i] = test_y(0,i);
  }

  io_store_pdf (cdf);
  pdf_to_cdf (cdf);
  io_store_cdf (cdf);
}

void NeuralNetwork::destroy () {
  pdf_fid.close();
  cdf_fid.close();
}