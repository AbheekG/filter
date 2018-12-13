#include "filter.hh"
#include <cmath>

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

	// Storing Error.
	path = test_path + "error-state-mean.csv";
	error_state_mean_fid.open (path.c_str());
	if (!error_state_mean_fid) cout << "Couldn't open the file.\n";
	path = test_path + "error-state-mode.csv";
	error_state_mode_fid.open (path.c_str());
	if (!error_state_mode_fid) cout << "Couldn't open the file.\n";
	path = test_path + "error-cdf.csv";
	error_cdf_fid.open (path.c_str());
	if (!error_cdf_fid) cout << "Couldn't open the file.\n";
}

void Filter::io_destroy () {
	motion_fid.close ();
	sensor_fid.close ();
	error_state_mean_fid.close ();
	error_state_mode_fid.close ();
	error_cdf_fid.close ();
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

void Filter::io_store_error (const vector<double> &cdf) {
	double mean_error = 0, mode_error = 0, cdf_error = 0;

	for (int i = 0; i < n_dim; ++i) {
		mean_error += (( (*base_mean_state)[i] - mean_state[i] )
			*( (*base_mean_state)[i] - mean_state[i] ));
	}

	for (int i = 0; i < n_dim; ++i) {
		mode_error += (( (*base_mode_state)[i] - mode_state[i] )
			*( (*base_mode_state)[i] - mode_state[i] ));
	}

	for (int i = 0; i < cdf.size(); ++i) {
		cdf_error += (( (*base_cdf)[i] - cdf[i] )
			*( (*base_cdf)[i] - cdf[i] ));
	}

	error_state_mean_fid << std::fixed << std::setprecision(4) << sqrt(mean_error) << " & ";
	error_state_mode_fid << std::fixed << std::setprecision(4) << sqrt(mode_error) << " & ";
	error_cdf_fid << std::fixed << std::setprecision(4) << sqrt(cdf_error) << " & ";
}


void Filter::io_diffuse_pdf (vector<double> &pdf) {

	// Summing up to find PDF.
	/*
	TODO.
	Currently supports 1-D, 2-D and 3-D individually.
	Add support for arbritary dimension.
	*/
	vector<int> index(n_dim);
	vector<int> shifts = {-3, -2, -1, 0, 1, 2, 3};

	assert (n_dim == 1 || n_dim == 2 || n_dim == 3);
	int iii = 50;
	while (iii > 0) {
		iii--;
		auto temp_pdf = pdf;
		if (n_dim == 1) {
			int i = 0;
			for (index[i] = 0; index[i] < dim_size[i]; ++index[i]) {
				
				double count = 0, wt = 0;
				for (auto shift : shifts) {
					index[i] += shift;
					bool valid_index = true;
					for (int i_dim = 0; i_dim < n_dim; ++i_dim) {
						if (index[i_dim] < 0) valid_index = false;
						if (index[i_dim] >= dim_size[i_dim]) valid_index = false;
					}
					if (valid_index) {
						wt += temp_pdf[index_to_id(index)];
						count += 1.0;
					}
					index[i] -= shift;
				}
				pdf[index_to_id(index)] = wt/count;

			}
		}
		else if (n_dim == 2) {
			int i = 0, j = 1;
			for (index[i] = 0; index[i] < dim_size[i]; ++index[i]) {
				for (index[j] = 0; index[j] < dim_size[j]; ++index[j]) {
					
					double count = 0, wt = 0;
					for (auto i_shift : shifts) {
					for (auto j_shift : shifts) {
						index[i] += i_shift;
						index[j] += j_shift;
						bool valid_index = true;
						for (int i_dim = 0; i_dim < n_dim; ++i_dim) {
							if (index[i_dim] < 0) valid_index = false;
							if (index[i_dim] >= dim_size[i_dim]) valid_index = false;
						}
						if (valid_index) {
							wt += temp_pdf[index_to_id(index)];
							count += 1.0;
						}
						index[i] -= i_shift;
						index[j] -= j_shift;
					}}
					pdf[index_to_id(index)] = wt/count;

				}
			}
		}
		else if (n_dim == 3) {
			int i = 0, j = 1, k = 2;
			for (index[i] = 0; index[i] < dim_size[i]; ++index[i]) {
				for (index[j] = 0; index[j] < dim_size[j]; ++index[j]) {
					for (index[k] = 0; index[k] < dim_size[k]; ++index[k]) {
						
						double count = 0, wt = 0;
						for (auto i_shift : shifts) {
						for (auto j_shift : shifts) {
						for (auto k_shift : shifts) {
							index[i] += i_shift;
							index[j] += j_shift;
							index[k] += k_shift;
							bool valid_index = true;
							for (int i_dim = 0; i_dim < n_dim; ++i_dim) {
								if (index[i_dim] < 0) valid_index = false;
								if (index[i_dim] >= dim_size[i_dim]) valid_index = false;
							}
							if (valid_index) {
								wt += temp_pdf[index_to_id(index)];
								count += 1.0;
							}
							index[i] -= i_shift;
							index[j] -= j_shift;
							index[k] -= k_shift;
						}}}
						pdf[index_to_id(index)] = wt/count;

					}
				}
			}
		}
	}
}