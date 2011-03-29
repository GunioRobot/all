#include <cstdlib>
#include <ctime>
#include <sys/time.h>

namespace dmath {
	double urandom(double modifier = 1) { //unsigned
		return (((double)std::rand() / (double)RAND_MAX) * modifier);
	}
	double random(double modifier = 1) { //signed
		return (-modifier + (2 * urandom(modifier)));
	}
	void randomseed() { //based on sys (microsec) time
		timeval a;
		gettimeofday(&a, 0);
		std::srand(a.tv_sec * (unsigned int)1e6 + a.tv_usec);
	}
}
