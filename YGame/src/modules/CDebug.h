#ifndef _Crucial_Debug_H_
	#define _Crucial_Debug_H_

#include <fstream>

namespace crucial {
	class CDebug {
		std::ofstream stream;
		std::streambuf* cdebug_buf;
		std::streambuf* cout_buf;
		std::streambuf* cerr_buf;
		std::streambuf* clog_buf;
		
	public:
		CDebug(const char* file);
		~CDebug();
	};
}

#endif
