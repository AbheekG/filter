#include "filter.hh"
#include "GridFilter.hh"
#include <algorithm>

// size is the number of particles in each dimension
void GridFilter::init (int grid_approx) {

  grid_size.resize(get_dim());
  for (int i = 0; i < get_dim(); ++i) {
    grid_size[i] = (get_size(i)-1)/grid_approx + 1;
  }  

  grid_total_size = 1;
  for (auto gs : grid_size) grid_total_size *= gs;

  grid_points.resize(get_dim());
  for (int i = 0; i < get_dim(); ++i) {
    grid_points[i].resize(grid_size[i]);
    for (int j = 0; j < grid_size[i]; j++) {
      grid_points[i][j] = get_bound(LOWER, i) + (get_bound(UPPER, i) 
        - get_bound(LOWER, i)) * (1./ (grid_size[i]-1)) * j;
    }
  }

  // Uniform prior
  grid_pdf.resize(grid_total_size);
  for (auto &gp : grid_pdf) gp = 1.0/grid_total_size;

  cdf.resize(get_total_size());
  // CDF I/O
  name = "grid-filter-" + std::to_string(grid_approx) + ".csv";
  string path = get_test_path() + "cdf-" + name;
  cdf_fid.open(path);
  path = get_test_path() + "pdf-" + name;
  pdf_fid.open(path);
}

void GridFilter::process () {
  clock_start ();

  /*
  // Fast inplace motion update INCOMPLETE
  
  // Using the 0 state we get the motion.
  state_t update(get_dim(), 0);
  update = motion_update (update);

  // If robot moved in positive direction, say 5 unit, then
  // the probability at 0 goes to 5. While updating we need
  // to go from MAX to MIN to prevent overwrite. sign < 0.
  vector<int> sign(get_dim(), 0);
  for (int i = 0; i < get_dim(); ++i) {
    sign[i] = (update[i] > 0 ? -1 : 1);
  }
  */

  // Motion update SLOW
  vector<double> old_grid_pdf(grid_total_size, 0);

  // TEMP
  int out_of_range = 0;

  for (int i = 0; i < grid_total_size; ++i) {
    auto state = grid_index_to_state( grid_id_to_index (i) );

    //TEMP
    // auto index = grid_id_to_index (i);
    // for (auto id : index) cout << id << " "; cout << endl;
    // for (auto st : state) cout << st << " "; cout << endl;

    state = motion_update(state);
    //TEMP
    // for (auto st : state) cout << st << " "; cout << endl;

    bool in_range = true;
    for (int i = 0; i < get_dim(); ++i) {
      if (state[i] < get_bound(LOWER, i) || state[i] > get_bound(UPPER, i)) {
        in_range = false;
        break;
      }
    }

    if (in_range) {
      old_grid_pdf[grid_index_to_id(grid_state_to_index(state))] = grid_pdf[i];
    } else {
      out_of_range++;
    }
  }

  // cout << "Out of range = " << out_of_range << endl;

  // Sensor update
  // Calculating weights
  for (int i = 0; i < grid_total_size; ++i) {
    grid_pdf[i] = old_grid_pdf[i] * sensor_update ( 
      grid_index_to_state (grid_id_to_index(i)) );
    // grid_pdf[i] += 1. / (grid_total_size * grid_total_size);
  }

  // Normalizing.
  double sum = 0;
  for (const auto &gp : grid_pdf) sum += gp;
  for (auto &gp : grid_pdf) gp /= sum;

  clock_stop ();
}

void GridFilter::store_cdf () {

  std::fill(cdf.begin(), cdf.end(), 0);

  // Construct approximate pdf. here pdf is called cdf.
  for (int i = 0; i < grid_total_size; ++i) {
    cdf[index_to_id( state_to_index( grid_index_to_state( 
      grid_id_to_index (i))))] += grid_pdf[i];
  }

  io_store_pdf (cdf);
  compute_mean_state (cdf);
  pdf_to_cdf (cdf);
  io_store_cdf (cdf);
  io_store_error (cdf);
}

void GridFilter::destroy () {
  pdf_fid.close();
  cdf_fid.close();
}