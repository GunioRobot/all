#include <iostream>
#include <cassert>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/microsec_time_clock.hpp>

class TestTimer {
public:
	TestTimer(const std::string & name) : name(name), start(boost::date_time::microsec_clock<boost::posix_time::ptime>::local_time()) {}
	~TestTimer() {
		using namespace std;
		using namespace boost;

		posix_time::ptime now(date_time::microsec_clock<posix_time::ptime>::local_time());
		posix_time::time_duration d = now - start;

		cout << name << " completed in " << d.total_milliseconds() / 1000.0 << " seconds" << endl;
	}
private:
	std::string name;
	boost::posix_time::ptime start;
};

std::vector<double> values = 	{
					1,2,3,7,4,2,7,3,234,7,23,35,68,2,34,68,3,2354,68,345,6,6,2345,678,3,23,23,3,23,4,3,42,432,4365,34,234,345,623,432543,324,233,35,23,
					1,2,3,7,4,2,7,3,234,7,23,35,68,2,34,68,3,2354,68,345,6,6,2345,678,3,23,23,3,23,4,3,42,432,4365,34,234,345,623,432543,324,233,35,23,
					1,2,3,7,4,2,7,3,234,7,23,35,68,2,34,68,3,2354,68,345,6,6,2345,678,3,23,23,3,23,4,3,42,432,4365,34,234,345,623,432543,324,233,35,23,
					1,2,3,7,4,2,7,3,234,7,23,35,68,2,34,68,3,2354,68,345,6,6,2345,678,3,23,23,3,23,4,3,42,432,4365,34,234,345,623,432543,324,233,35,23,
					1,2,3,7,4,2,7,3,234,7,23,35,68,2,34,68,3,2354,68,345,6,6,2345,678,3,23,23,3,23,4,3,42,432,4365,34,234,345,623,432543,324,233,35,23
				};

class Average {
private:
	size_t num;
	double sum;
public:
	Average() : num (0) , sum (0) {}
	void operator()(double elem) { num++; sum += elem; }
	operator double() const { return sum / num; }
};

double CalculateAverage(const double* begin, const double* end) {
	double sum = 0;
	for(size_t i = 0; i < (end - begin); ++i) {
		sum += begin[i];
	}
	return sum / (end - begin);
}

double avg = 0;
const size_t iterations = 4e7;

void UseFunctor() {
	TestTimer t("UseFunctor");
	#pragma omp parallel for
	for (size_t i = 0; i < iterations; ++i) {
		double average = std::for_each(values.begin(), values.end(), Average());
		avg += average;
	}
}

void UseFunction() {
	TestTimer t("UseFunction");
	#pragma omp parallel for
	for (size_t i = 0; i < iterations; ++i) {
		double average = CalculateAverage(&values[0], (&values.back()) + 1);
		avg += average;
	}
}

struct RD {
	bool a;
	int b;
};

struct RS {
	bool operator() (const RD& a, const RD& b) {
		return (a.a == true) && (b.a == false);
	}
};

std::vector<RD> rds;
RS rs;

int main() {
	RD x = {true, 343};
	RD y = {false, 641};

	for (int i = 0; i < 10; ++i) {
		rds.push_back(x);
		rds.push_back(y);
	}


	std::sort(rds.begin(), rds.end(), rs);

	for (int i = 0; i < rds.size(); ++i) {
		printf("%i%i\n", rds[i].a, rds[i].b);
	}

	std::string abc("abc");
	std::string defgh("defgh");

	for (size_t i = 0; i < defgh.length(); ++i)
	{
	    printf("%c", abc[i % abc.length()]);
	}

	//std::cout << "Average value: " << avg / iterations / 2 << '\n';
	return 0;
}

