#include "filter.hh"

/*
Input/Output.
Plot data and function estimated error related.
*/

void Filter::io_init () {

	string path;

	// Processing meta-data
	path = test_path + "meta.csv";
	meta_fid.open (path.c_str());
	if (!meta_fid) cout << "Couldn't open the file.\n";

	meta_fid >> n_epoch;
	meta_fid >> n_dim;

	dim_size.resize(n_dim);
	for (auto &dim : dim_size) {
		meta_fid >> dim;
	}
	
	bounds.resize(2);
	bounds[LOWER].resize(n_dim);
	bounds[UPPER].resize(n_dim);
	for (int i = 0; i < n_dim; ++i) {
		meta_fid >> bounds[LOWER][i] >> bounds[UPPER][i];
	}

	total_size = 1;
	for (auto dim : dim_size) {
		total_size *= dim;
	}
	
	domain_points.resize(total_size);
	for (auto& point : domain_points) {
		for (auto &pt : point) {
			meta_fid >> pt;
		}
	}
	
	meta_fid.close ();

	// Motion.
	path = test_path + "motion.csv";
	motion_fid.open (path.c_str());
	if (!motion_fid) cout << "Couldn't open the file.\n";

	// Sensor.
	path = test_path + "sensor.csv";
	sensor_fid.open (path.c_str());
	if (!sensor_fid) cout << "Couldn't open the file.\n";
}

void Filter::io_destroy () {
	motion_fid.close ();
	sensor_fid.close ();
}