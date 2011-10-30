#ifndef _Crucial_INIParser_H_
	#define _Crucial_INIParser_H_

#include <map>
#include <string>

namespace crucial {
	class CINIParser {
		std::map<std::string, std::string> values;
		
	public:
		CINIParser(const char* filepath);

		void add(std::string section, std::string name, std::string value);
		std::string get(std::string section, std::string name, const std::string defaultv);
		int get(std::string section, std::string name, const int defaultv);
	};
}

#endif
