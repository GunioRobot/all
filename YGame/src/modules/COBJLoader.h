#ifndef _C_OBJLoader_H_
	#define _C_OBJLoader_H_

#include <fstream>

#include <boost/shared_ptr.hpp>
#include "CMesh.h"

namespace crucial {
	class COBJLoader {
		std::ifstream stream;

		void handle();
		void skip();

		CMesh_ptr mesh;
	public:
		std::vector<CMesh_ptr> meshes;

		COBJLoader(const char* file);
	};
}

#endif
