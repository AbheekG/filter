/*
This file contains code for the sensor update.
And some basic functions for array access.
And some other functions.
*/

#include "filter.hh"

double Filter::sensor_update (state_t x) {
	return sensor_pdf[index_to_id(state_to_index (x))];
}

void Filter::pdf_to_cdf (vector<double> &cdf) {

	// Summing up to find PDF.
	/*
	TODO.
	Currently supports 1-D, 2-D and 3-D individually.
	Add support for arbritary dimension.
	*/
	vector<int> index(n_dim);

	assert (n_dim == 1 || n_dim == 2 || n_dim == 3);
	double temp = 0;
	if (n_dim == 1) {
		int i = 0;
		for (index[i] = 0; index[i] < dim_size[i]; ++index[i]) {
			cdf[index_to_id(index)] += temp;
			temp = cdf[index_to_id(index)];
		}
	}
	else if (n_dim == 2) {
		int i = 0, j = 1;
		for (index[i] = 0; index[i] < dim_size[i]; ++index[i]) {
			for (index[j] = 0; index[j] < dim_size[j]; ++index[j]) {
				cdf[index_to_id(index)] += temp;
				temp = cdf[index_to_id(index)];
			}
		}
	}
	else if (n_dim == 3) {
		int i = 0, j = 1, k = 2;
		for (index[i] = 0; index[i] < dim_size[i]; ++index[i]) {
			for (index[j] = 0; index[j] < dim_size[j]; ++index[j]) {
				for (index[k] = 0; index[k] < dim_size[k]; ++index[k]) {
					cdf[index_to_id(index)] += temp;
					temp = cdf[index_to_id(index)];
				}
			}
		}
	}
}

void Filter::compute_mean_state (const vector<double> &pdf) {

	double wt = 0;
	state_t state(n_dim, 0);
	mean_state.resize(n_dim, 0);

	for (int i = 0; i < pdf.size(); ++i) {
		state = index_to_state( id_to_index (i) );
		for (int j = 0; j < n_dim; ++j) {
			mean_state[j] += pdf[i] * state[j];
		}
	}
}


void Filter::compute_mode_state (const vector<double> &pdf) {

	double wt = 0;
	state_t state(n_dim, 0);
	mode_state.resize(n_dim, 0);

	for (int i = 0; i < pdf.size(); ++i) {
		state = index_to_state( id_to_index (i) );
		if (wt < pdf[i]) {
			wt = pdf[i];
			mode_state = state;
		}
	}
}