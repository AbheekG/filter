#include "filter.hh"
#include "ParticleFilter.hh"
#include "GridFilter.hh"

int main () {
	// Parameters.
	string TEST = "data/test1/";

	Filter::filter_init (TEST);
	// Filter::filter_print ();

	// Particle filter
	int N_PARTICLES = 10000;
	ParticleFilter pf;
	pf.init(N_PARTICLES);

	// Fixed Grid Filter
	int grid_approx = 8;	// storing 1/8 times data each dimension.
	vector<int> grid_size(Filter::get_dim());
	for (int i = 0; i < Filter::get_dim(); ++i) {
		grid_size[i] = (Filter::get_size(i)-1)/grid_approx + 1;
		cout << grid_size[i] << " ";
	}
	GridFilter gf;
	gf.init(grid_size);

	while (Filter::get_iteration() < Filter::get_total_iteration()) {
		cout << "Iteration: " << Filter::get_iteration() << endl;
		Filter::new_iteration();

		pf.process ();
		cout << "Particle filter: " << pf.get_clock_time () << endl;
		pf.store_cdf();

		gf.process ();
		cout << "Fixed grid filter: " << gf.get_clock_time () << endl;
		gf.store_cdf ();
	}

	Filter::filter_destroy ();
	return 0;
}