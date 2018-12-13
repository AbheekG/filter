#pragma once

#include "filter.hh"
#include <Eigen/Core>
#include <Eigen/Dense>

using std::vector;

class PolyFilter : public Filter {
	
	string name;
	
	typedef Eigen::MatrixXd Matrix;

	state_t motion_offset;

	Matrix train_x;
	Matrix train_y;

	Matrix coeff;
	
	int deg;
	int new_dim;
	int n_samples;

	// noNAO
	Matrix test_x;
  	Matrix test_y;

  	// Normalize output
	void normalize (Matrix &train_y);
	vector<double> poly_state (const state_t &);

public:
	void init (int, int);
	void process ();
	state_t current_state ();
	// Processes and stores the CDF.
	void store_cdf ();
	void destroy ();
};
