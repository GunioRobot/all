#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include <iostream>

class ScopeTimer {
	std::string name;
	boost::posix_time::ptime start;
public:
	ScopeTimer(const std::string& name) : name(name), start(boost::date_time::microsec_clock<boost::posix_time::ptime>::local_time()) {}
	~ScopeTimer() {
		using namespace boost;

		posix_time::ptime now(date_time::microsec_clock<posix_time::ptime>::local_time());
		posix_time::time_duration d = now - start;

		std::cout << name << " completed in " << d.total_milliseconds() << "ms" << std::endl;
	}
};
