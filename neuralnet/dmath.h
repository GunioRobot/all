#ifndef _dmath_h_
	#define _dmath_h_

#include <cmath>

namespace dmath {
	double urandom(double modifier = 1); //unsigned
	double random(double modifier = 1); //signed
	void randomseed();

	double mean_abs_error(int n_base,double *base,double *comparison);
}

#endif
