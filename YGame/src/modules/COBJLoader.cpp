#include "OBJ.h"
#include "COBJLoader.h"

#include <iostream>
#include <cctype>

namespace crucial {
	COBJLoader::COBJLoader(const char* file) {
		stream.open(file, std::ios::in);
		if (stream) {
			mesh.reset(new CMesh);
			mesh->name = "";
			meshes.push_back(mesh);

			while (!stream.eof()) {
				handle();
				if (stream.bad() || stream.fail()) {
					//throw error;
					std::cerr << "Error parsing " << file << '\n';
					break;
				}
			}
			stream.close();
		} else {
			//throw error;
			std::cerr << "Could not open " << file << '\n';
		}
	}
	void COBJLoader::handle() {
		char prefix[10] = {0};
		stream >> prefix;

		switch (prefix[0]) {
			case OBJ_VERTEX : {
				switch (prefix[1]) {
					case VERT_TEXTURE : {} break;
					case VERT_NORMAL : {} break;
					default : {
						float x, y, z;
						stream >> x >> y >> z;

						mesh->vertices.push_back(x);
						mesh->vertices.push_back(y);
						mesh->vertices.push_back(z);
					}
				}
			} break;
			case OBJ_FACE : {
				unsigned short a, b, c;
				stream >> a >> b >> c;

				mesh->indices.push_back(a);
				mesh->indices.push_back(b);
				mesh->indices.push_back(c);
			} break;
			default : skip();
		}
	}
	void COBJLoader::skip() {
		char tmp[1024];
		stream.getline(tmp, 1024);
	}
}
