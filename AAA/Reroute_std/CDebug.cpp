#include <crucial/CDebug.h>

#include <iostream>

namespace crucial {
	CDebug::CDebug(const char* file) {
		stream.open(file);
		if (stream) {
			cdebug_buf = stream.rdbuf();
			cout_buf = std::cout.rdbuf();
			cerr_buf = std::cerr.rdbuf();
			clog_buf = std::clog.rdbuf();

			std::cout.rdbuf(cdebug_buf);
			std::clog.rdbuf(cdebug_buf);
			std::cerr.rdbuf(cdebug_buf);
		}
	}
	CDebug::~CDebug() {
		std::cout.rdbuf(cout_buf);
		std::clog.rdbuf(cerr_buf);
		std::cerr.rdbuf(clog_buf);
	}
}
