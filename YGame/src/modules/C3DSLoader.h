#ifndef _C_3DSLoader_H_
	#define _C_3DSLoader_H_

#include <boost/shared_ptr.hpp>
#include "CMesh.h"

#include <fstream>
#include <string>
#include <vector>

namespace crucial {
	class C3DSLoader {
		std::ifstream stream;
		unsigned int id;
		unsigned long length, offset;

		bool finished;
		std::string name;

		CMesh_ptr mesh;

		void handle();
		void parse();

		void enter(bool has_dta = false);
		void readbytes(void* ptr, size_t bytes);
		void skip();

		std::string getstr();
		void skipstr();

	public:
		std::vector<CMesh_ptr> meshes;

		C3DSLoader(const char* file);
	};
}

#endif
