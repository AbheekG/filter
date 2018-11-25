#pragma once

#include <vector>
#include "filter.hh"

using std::vector;

class ParticleFilter : public Filter {
	int n_particles;
	vector<state_t> particles;

	void init_particles ();

public:
	void init (int, vector<state_t>, int);
	void process ();
	state_t current_state ();
};
