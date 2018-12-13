#include "filter.hh"

// Static variables init.
string Filter::test_path;
std::ifstream Filter::meta_fid;
std::ifstream Filter::motion_fid;
std::ifstream Filter::sensor_fid;
std::ofstream Filter::state_error_fid;
std::ofstream Filter::cdf_error_fid;
vector<state_t> Filter::domain_points;

int Filter::n_epoch;
int Filter::epoch;
state_t Filter::motion_u;
vector<double> Filter::sensor_pdf;

int Filter::n_dim;
int Filter::total_size;
vector<int> Filter::dim_size;
vector<state_t> Filter::dim_bounds;
state_t Filter::dim_density;

state_t* Filter::base_mean_state = NULL;
vector<double>* Filter::base_cdf = NULL;

// Init.
void Filter::filter_init  (string _test_path = "data/test1/") {
	
	test_path = _test_path;

	io_init ();

	motion_u.resize (n_dim);
	sensor_pdf.resize (total_size);

	epoch = 0;
}


void Filter::make_base () {
	base_mean_state = &mean_state;
	base_cdf = &cdf;
}

void Filter::filter_destroy () {
	io_destroy ();
}

void Filter::filter_print () {

	cout << "Test path: " << test_path << endl;
	cout << "# epochs: " << n_epoch << endl;
	cout << "# dimensions: " << n_dim << endl;
	cout << "# total size: " << total_size << endl;
	
	cout << "Dim sizes: ";
	for (const auto &dim : dim_size) cout << dim << ", ";
	
	cout << "\nDim bounds. ";
	cout << "\nLower:\t";
	for (const auto &b : dim_bounds[LOWER]) cout << b << "\t";
	cout << "\nUpper:\t";
	for (const auto &b : dim_bounds[UPPER]) cout << b << "\t";

	cout << "\nPoints in domain along dimensions.";
	for (const auto &dp : domain_points) {
		cout << endl;
		for (const auto &p : dp) {
			cout << p << "\t";
		}
	}

	cout << endl;
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

	state_error_fid << endl << epoch << " & ";
	cdf_error_fid << endl << epoch << " & ";
}

/*
Clock related.
*/

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