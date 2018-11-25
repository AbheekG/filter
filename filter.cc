#include "filter.hh"

int main () {
	return 0;
}


// Init.
void Filter::base_filter_init  (int dim, vector<state_t> input_bounds) {
	epoch = 0;
	n_dim = dim;
	bounds = input_bounds;
}

// Iteration.
void Filter::new_iteration () {
	epoch++;
}

int Filter::get_iteration () {
	return epoch;
}

// Dimension and bounds.
int Filter::get_dim () {
	return n_dim;	
}

double Filter::get_bound (int lu, int dim) {
	return bounds[lu][dim];
}

/*
Plot and function estimated error related.
*/



/*
Clock related.
*/
std::clock_t Filter::clock_time () {
	return time_duration;
}

void Filter::clock_start () {
	time_duration = 0;
	time = std::clock();
}

void Filter::clock_stop () {
	time_duration += std::clock() - time;
}

void Filter::clock_resume () {
	time = std::clock();
}