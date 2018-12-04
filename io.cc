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
	
	dim_bounds.resize(2);
	dim_bounds[LOWER].resize(n_dim);
	dim_bounds[UPPER].resize(n_dim);
	for (int i = 0; i < n_dim; ++i) {
		meta_fid >> dim_bounds[LOWER][i] >> dim_bounds[UPPER][i];
	}

	dim_density.resize(n_dim);
	for (int i = 0; i < n_dim; ++i) {
		dim_density[i] = dim_size[i] / (dim_bounds[UPPER][i] - dim_bounds[LOWER][i]);
	}

	total_size = 1;
	for (auto dim : dim_size) {
		total_size *= dim;
	}
	
	domain_points.resize(n_dim);
	for (int i = 0; i < n_dim; ++i) {
		domain_points[i].resize (dim_size[i]);
		for (auto &pt : domain_points[i]) {
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

void Filter::io_store_cdf (vector<double> &cdf) {
	for (int i = 0; i < cdf.size(); i++) {
		cdf_fid << cdf[i];
		if (i < cdf.size() - 1) cdf_fid << " "; else cdf_fid << endl;
	}
}

void Filter::io_store_pdf (vector<double> &pdf) {
	for (int i = 0; i < pdf.size(); i++) {
		pdf_fid << pdf[i];
		if (i < pdf.size() - 1) pdf_fid << " "; else pdf_fid << endl;
	}
}