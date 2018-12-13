/*
This file contains code for the motion update.
*/

#include "filter.hh"

state_t Filter::motion_update (state_t x) {
	for (int i = 0; i < n_dim; ++i) {
		x[i] += motion_u[i];
	}
	return x;
}