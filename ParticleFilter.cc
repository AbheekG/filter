#include "filter.hh"
#include "ParticleFilter.hh"
#include <algorithm>

// size is the number of particles in each dimension
void ParticleFilter::init (int n_part) {
   
  // CDF I/O
  name = "particle_filter_cdf.csv";
  string path = get_test_path() + name;
  cdf_fid.open(path);

  n_particles = n_part;
  particles.resize(n_particles);
  for (auto &particle : particles) {
    particle.resize(get_dim());
  }

  // Init Particles.
  init_particles ();
}

void ParticleFilter::init_particles () {
  for (int i_dim = 0; i_dim < get_dim(); ++i_dim) {
    std::uniform_real_distribution<double> distribution(get_bound(LOWER, i_dim), get_bound(UPPER, i_dim));
    for (auto &particle : particles) {
      particle[i_dim] = distribution(generator);
    }
  }
}

void ParticleFilter::process () {
  clock_start ();

  vector<double> weights(n_particles, 0);
  vector<state_t> old_particles(n_particles);

  // Motion update
  for (int i = 0; i < n_particles; ++i) {
    old_particles[i] = motion_update (particles[i]);
  }

  // Sensor update
  // Calculating weights
  for (int i = 0; i < n_particles; ++i) {
    weights[i] = sensor_update (old_particles[i]);
  }
  
  for (int i = 1; i < n_particles; ++i) {
    weights[i] += weights[i-1];
  }

  // Resampling.
  std::uniform_real_distribution<double> distribution(0, weights[n_particles-1]);
  for (int i = 1; i < n_particles; ++i) {
    double rand = distribution(generator);
    int j = std::lower_bound (weights.begin(), weights.end(), rand) - weights.begin();
    particles[i] = old_particles[j];
  }

  clock_stop ();  
}

void ParticleFilter::store_cdf () {
  
  static vector<double> cdf(get_total_size());
  std::fill(cdf.begin(), cdf.end(), 0);

  // Construct approximate pdf. here pdf is called cdf.
  for (const auto &p : particles) {
    cdf[index_to_id(state_to_index (p))] += 1.0/n_particles;
  }

  pdf_to_cdf (cdf);
  io_store_cdf (cdf);
}

void ParticleFilter::destroy () {
  cdf_fid.close();
}