/*
This file contains code for the sensor update.
*/

#include "filter.hh"

double Filter::sensor_update (state_t x) {
	vector<int> index;
	index.push_back (0);
	return sensor_pdf_get(index);
}

double Filter::sensor_pdf_get (vector<int> index) {
	int id = 0;
	for (int i = 0; i < n_dim; ++i) {
		id = id*dim_size[i] + index[i];
	}
	return sensor_pdf[id];
}