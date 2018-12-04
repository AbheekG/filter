#pragma once

#include <vector>
#include "MiniDNN/MiniDNN.h"
#include "filter.hh"

using std::vector;

class NeuralNetwork : public Filter {
	
	string name;
	
	typedef Eigen::MatrixXd Matrix;

	state_t motion_offset;

	Matrix train_x;
	Matrix train_y;
	
	int nn_iter;
	int nn_samples;
	vector<int> layer_sizes;

	// Neural net
	MiniDNN::Network net;
	vector<MiniDNN::Layer*> layers;
	MiniDNN::SGD opt;
	MiniDNN::Callback callback;

	// noNAO
	Matrix test_x;
  	Matrix test_y;


public:
	void init (int, int, vector<int>, double);
	void process ();
	state_t current_state ();
	// Processes and stores the CDF.
	void store_cdf ();
	void destroy ();
};
