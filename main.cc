#include "filter.hh"
#include "ParticleFilter.hh"

int N_PARTICLES = 100;
string TEST = "data/test1/";

int main () {
	Filter::filter_init (TEST);

	// Particle filter
	ParticleFilter pf;
	pf.init(N_PARTICLES);

	while (Filter::get_iteration() < Filter::n_iteration()) {
		cout << Filter::get_iteration() << endl;
		Filter::new_iteration();
	}

	Filter::filter_destroy ();
	return 0;
}