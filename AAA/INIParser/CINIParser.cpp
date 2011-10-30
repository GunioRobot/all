#include <crucial/CINIParser.h>

#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>

namespace crucial {
	CINIParser::CINIParser(const char* filepath) {
		std::ifstream stream(filepath);
		std::string str, section, name = "name", value;
		int end;

		if (stream) {
			while (std::getline(stream, str)) {
				boost::trim(str);
				if (str[0] == '[') {
					//section
					end = str.find(']') - 1;
					if (end) {
						section = str.substr(1,end);
					}
				} else if (not (str[0] == ';' or str[0] == '#') and (str.length() and str.find('='))) {
					//name
					end = str.find('=');
					name = str.substr(0,end);
					boost::trim(name);
					//value
					value = str.substr(end + 1,str.length());
					end = value.find(';');
					if (end) {
						value = value.substr(0,end);
					}
					boost::trim(value);
					if (name.length() and value.length()) {
						add(section,name,value);
					}
				}
			}
		}
	}

	void CINIParser::add(std::string section, std::string name, std::string value) {
		std::string key = section + name;
		boost::to_lower(key);
		values[key] = value;
		return;
	}

	std::string CINIParser::get(std::string section, std::string name, const std::string defaultv) {
		std::string key = section + name;
		boost::to_lower(key);
		if (values.count(key)) {
			return values[key];
		}
		return defaultv;
	}

	int CINIParser::get(std::string section, std::string name, const int defaultv) {
		std::string value = get(section,name,"");
		if (!value.length()) {
			return defaultv;
		}
		int result;
		std::stringstream(value) >> result;
		return result;
	}
}
