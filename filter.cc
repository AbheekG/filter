#include "filter.hh"

// Static variables init.
string Filter::test_path;
std::ifstream Filter::meta_fid;
std::ifstream Filter::motion_fid;
std::ifstream Filter::sensor_fid;
vector<state_t> Filter::domain_points;

int Filter::n_epoch;
int Filter::epoch;
state_t Filter::motion_u;
vector<double> Filter::sensor_pdf;

int Filter::n_dim;
int Filter::total_size;
vector<int> Filter::dim_size;
vector<state_t> Filter::bounds;


// Init.
void Filter::filter_init  (string _test_path = "data/test1/") {
	
	test_path = _test_path;

	io_init ();

	motion_u.resize (n_dim);
	sensor_pdf.resize (total_size);

	epoch = 0;
}

void Filter::filter_destroy () {
	io_destroy ();
}

// Iteration.
void Filter::new_iteration () {
	
	epoch++;

	// Update motion and sensor data.
	// Motion.
	for (auto &u : motion_u) {
		motion_fid >> u;
	}
	// Sensor
	for (auto &p : sensor_pdf) {
		sensor_fid >> p;
	}
}

int Filter::n_iteration () {
	return n_epoch;
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