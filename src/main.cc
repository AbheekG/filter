// R.S.
#include "filter.hh"
#include "ParticleFilter.hh"
#include "GridFilter.hh"
#include "DynamicGridFilter.hh"
#include "PolyFilter.hh"
#include "NeuralNetwork.hh"

int main () {
	// Parameters.
	string TEST = "data/test3/";

	Filter::filter_init (TEST);
	// Filter::filter_print ();

	// Baseline Grid Filter
	int base_grid_approx = 2;
	cout << "Base grid filter.\n\tUsing grid approx ratio = " << base_grid_approx << endl;
	GridFilter base_gf;
	base_gf.init(base_grid_approx);
	base_gf.make_base();

	// Particle filter
	int N_PARTICLES = 100;
	cout << "Particle filter.\n\tUsing # particles = " << N_PARTICLES << endl;
	ParticleFilter pf;
	pf.init(N_PARTICLES);

	// Fixed Grid Filter
	int grid_approx = 10;	// storing 1/10 times data each dimension.
	cout << "Fixed-grid filter.\n\tUsing grid approx ratio = " << grid_approx << endl;
	GridFilter gf;
	gf.init(grid_approx);

	// Dynamic Grid Filter
	double thr_l = 0.02, thr_h = 0.02;
	cout << "Dynamic-grid filter.\n";
	// cout << "Using lower threshold = " << thr_l << endl;
	cout << "\tUsing upper threshold = " << thr_h << endl;
	DynamicGridFilter dgf;
	dgf.init (thr_l, thr_h);

	// Poly Filter
	int polyf_samples = 100;
	int deg = 2;
	cout << "Polynomial filter.\n";
	cout << "\tUsing samples = " << polyf_samples << endl;
	cout << "\tUsing polynomial degree = " << deg << endl;
	PolyFilter polyf;
	polyf.init (polyf_samples, deg);

	// Neural Network
	int nn_iter = 1;	// # iterations in each step.
	int nn_samples = 100;	// # samples to use.
	vector<int> layer_sizes = {10,10};		// Layers.
	double l_rate = 0.1;	// Learning rate.
	cout << "Neural network with:\n";
	cout << "\t# iterations in each step = " << nn_iter << endl;
	cout << "\t# samples to use in each step = " << nn_samples << endl;
	cout << "\t# layers = " << layer_sizes.size() << "  { ";
	for (auto ls : layer_sizes) cout << ls << ", ";
	cout << "\n\tLearning rate = " << l_rate << endl;
	NeuralNetwork nnf;
	nnf.init (nn_iter, nn_samples, layer_sizes, l_rate);

	while (Filter::get_iteration() < Filter::get_total_iteration()) {
		cout << "\nIteration: " << Filter::get_iteration() << endl;
		Filter::new_iteration();

		base_gf.process ();
		cout << "Base grid filter: " << base_gf.get_clock_time () << endl;
		base_gf.store_cdf ();

		pf.process ();
		cout << "Particle filter: " << pf.get_clock_time () << endl;
		pf.store_cdf();

		gf.process ();
		cout << "Fixed grid filter: " << gf.get_clock_time () << endl;
		gf.store_cdf ();

		dgf.process ();
		cout << "Dynamic grid filter: " << dgf.get_clock_time () << endl;
		dgf.store_cdf ();

		polyf.process ();
		cout << "Polynomial (deg=2) filter: " << polyf.get_clock_time () << endl;
		polyf.store_cdf ();

		nnf.process ();
		cout << "Neural network filter: " << nnf.get_clock_time () << endl;
		nnf.store_cdf ();

		// int temp; cin >> temp;
	}

	base_gf.destroy();
	pf.destroy();
	gf.destroy();
	dgf.destroy();
	polyf.destroy();
	nnf.destroy();
	Filter::filter_destroy ();
	return 0;
}