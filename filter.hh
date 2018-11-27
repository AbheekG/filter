#pragma once

#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::clock_t;
using std::cin;
using std::cout;
using std::endl;

typedef vector<double> state_t;

class Filter {
private:
	// IO
	static string test_path;
	static std::ifstream meta_fid;
	static std::ifstream motion_fid;
	static std::ifstream sensor_fid;
	static void io_init ();
	static void io_destroy ();
	// Points in state space where we have the PDF value.
	static vector<state_t> domain_points;

	// Interation related.
	static int n_epoch;
	static int epoch;
	static state_t motion_u;
	static vector<double> sensor_pdf;
	static double sensor_pdf_get (vector<int>);

	// State related.
	static int n_dim;
	static int total_size;
	static vector<int> dim_size;
	static vector<state_t> bounds; // 0 : lower bound. 1 : upper bound.

	// Clock related.
	clock_t time, time_duration;

protected:
	const static int LOWER = 0;
	const static int UPPER = 1;

	// Clock related.
	void clock_start ();
	void clock_stop ();
	void clock_resume ();
	clock_t clock_time ();

	// Randomness related.
	std::default_random_engine generator;

	// Functions.
	// Access private data.
	int get_dim ();
	double get_bound (int, int);

	// Motion update. Definitons in motion_update.cc
	state_t motion_update (state_t);

	// Sensor update. Returns PDF value for given state. Definitons in sensor_update.cc
	double sensor_update (state_t);

public:
	// Fundamentals
	// Initiates general filter data and variables.
	static void filter_init (string);
	static void filter_destroy ();
	// Updates sensor data for new iteration.
	static void new_iteration ();
	static int n_iteration ();
	static int get_iteration ();

	// Processes and stores the CDF.
	static void store_base_cdf ();
	void store_cdf ();
};