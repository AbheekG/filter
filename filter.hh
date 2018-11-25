#pragma once

#include <random>
#include <ctime>
#include <vector>

using std::vector;
using std::clock_t;

typedef vector<double> state_t;

class Filter {
private:
	// Clock related.
	clock_t time, time_duration;

	// Interation related.
	int epoch;

	// State related.
	int n_dim;
	vector<state_t> bounds; // 0 : lower bound. 1 : upper bound.

protected:
	const int LOWER = 0;
	const int UPPER = 1;

	// Clock related.
	void clock_start ();
	void clock_stop ();
	void clock_resume ();
	clock_t clock_time ();

	// Randomness related.
	std::default_random_engine generator;

	// Functions
	void base_filter_init (int, vector<state_t>);
	void new_iteration ();
	int get_iteration ();
	int get_dim ();
	double get_bound (int, int);

	// Motion update. Definitons in motion_update.cc
	state_t motion_update (state_t);

	// Sensor update. Returns PDF value for given state. Definitons in sensor_update.cc
	double sensor_update (state_t);
};