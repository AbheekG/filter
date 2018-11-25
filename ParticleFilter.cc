#include "filter.hh"
#include "ParticleFilter.hh"
#include <algorithm>

// size is the number of particles in each dimension
void ParticleFilter::init (int dim, vector<state_t> input_bound, int size) {
  // Base init.
  base_filter_init (dim, input_bound);
  
  n_particles = 1;
  while (dim > 0) {
    n_particles *= size;
    dim--;
  }

  // Init Particles.
  init_particles ();
}

void ParticleFilter::init_particles () {
  for (int i_dim; i_dim < get_dim(); ++i_dim) {
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
    int j = std::upper_bound (weights.begin(), weights.end(), rand) - weights.begin();
    particles[i] = old_particles[j];
  }

  clock_stop ();  
}