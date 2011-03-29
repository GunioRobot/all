#include "3DS.h"
#include "C3DSLoader.h"

#include <iostream>

namespace crucial {
	C3DSLoader::C3DSLoader(const char* file) {
		id = 0; length = 0; offset = 0;
		finished = false;

		stream.open(file, std::ios::binary);
		if (stream) {
			while (!stream.eof()) {
				handle();
				if (stream.bad() || stream.fail()) {
					//throw error;
					std::cerr << "Error parsing " << file << '\n';
					break;
				}
				if (finished) break;
			}
			stream.close();
		} else {
			std::cerr << "Could not open " << file << '\n';
		}
	}
	void C3DSLoader::parse() {
		offset = stream.tellg();
		readbytes(&id, 2);
		readbytes(&length, 4);

		switch (id) {
			case MAIN3DS : enter(); break;
			case EDIT3DS : enter(); break;
				case EDIT_MATERIAL : enter(); break;
					case MAT_NAME : enter(1); break;
					case MAT_VCOORDS : enter(1); break;
					case MAT_UCOORDS : enter(1); break;
					case MAT_UOFFSET : enter(1); break;
					case MAT_VOFFSET : enter(1); break;

				case EDIT_OBJECT : enter(1); break;
					case OBJ_TRIMESH : enter(1); break;
						case TRI_VERTEXL : enter(1); break;
						case TRI_LOCAL : enter(1); break;
						case TRI_FACEL1 : enter(1); break;
							case TRI_FACE_MATERIAL : enter(1); break;
						case TRI_FACE_SMOOTHING : enter(1); break;
						case TRI_TEXCOORD : enter(1); break;
			default : skip(); break;
		}
	}
	void C3DSLoader::enter(bool has_dta) {
		unsigned long clength = offset + length;

		if (has_dta) parse();

		while ((unsigned)stream.tellg() < clength) {
			if (finished) break;
				else handle();
		}
	}
	void C3DSLoader::parse() {
		switch (id) {
			case MAT_NAME : {
				skipstr();
			} break;
			case MAT_TEXFILE : {
				skipstr();
			} break;
			case MAT_VCOORDS : {
				float vcoords = 0;
				readbytes(&vcoords, 4);
			} break;
			case MAT_UCOORDS : {
				float ucoords = 0;
				readbytes(&ucoords, 4);
			} break;
			case MAT_VOFFSET : {
				float voffset = 0;
				readbytes(&voffset, 4);
			} break;
			case MAT_UOFFSET : {
				float uoffset = 0;
				readbytes(&uoffset, 4);
			} break;

			case EDIT_OBJECT : {
				name = getstr();
			} break;
			case OBJ_TRIMESH : {
				mesh.reset(new CMesh);
				mesh->name = name;
				meshes.push_back(mesh);
			} break;
			case TRI_VERTEXL : {
				unsigned int n_vertices = 0;
				readbytes(&n_vertices, 2);
				n_vertices *= 3;

				mesh->vertices.resize(n_vertices);

				for (unsigned int i = 0; i < n_vertices; i += 3) {
					float x = 0, y = 0, z = 0;

					readbytes(&x, 4);
					readbytes(&y, 4);
					readbytes(&z, 4);

					mesh->vertices[i] = x;
					mesh->vertices[i+1] = y;
					mesh->vertices[i+2] = z;
				}
			} break;
			case TRI_FACEL1 : {
				unsigned int n_indices = 0;
				readbytes(&n_indices, 2);
				n_indices *= 3; // (4-1) unused 4th value

				mesh->indices.resize(n_indices);

				for (unsigned int i = 0; i < n_indices; i += 3) {
					unsigned short a = 0, b = 0, c = 0, f = 0;
					readbytes(&a, 2);
					readbytes(&b, 2);
					readbytes(&c, 2);
					readbytes(&f, 2);

					mesh->indices[i] = a;
					mesh->indices[i+1] = b;
					mesh->indices[i+2] = c;
					//mesh->indices[i+3] = f; //unused...
				}
			} break;
			case TRI_FACE_MATERIAL : { //unknown purpose
				skipstr();
				unsigned int n_faces = 0;
				readbytes(&n_faces, 2);

				for (unsigned int i = 0; i < n_faces; ++i) {
					unsigned int idx = 0;
					readbytes(&idx, 2);
					//add to a container
				}
			} break;
			case TRI_TEXCOORD : {
				unsigned int n_texcoords = 0;
				readbytes(&n_texcoords, 2);
				n_texcoords *= 2;

				mesh->texcoords.resize(n_texcoords);

				for (unsigned int i = 0; i < n_texcoords; i += 2) {
					float u = 0, v = 0;
					readbytes(&u, 4);
					readbytes(&v, 4);

					mesh->texcoords[i] = u;
					mesh->texcoords[i+1] = v;
				}
			} break;
			case TRI_FACE_SMOOTHING : { //unknown purpose
				unsigned long var = 0;
				unsigned int n_var = length - 6;
				for (unsigned int i = 0; i < n_var; i += 4) {
					readbytes(&var, 4);
				}
				break;
			}
			case TRI_LOCAL : {
				for (unsigned int i = 0; i < 4; ++i) {
					float x = 0, y = 0, z = 0;
					readbytes(&x, 4);
					readbytes(&y, 4);
					readbytes(&z, 4);
					//1st 3 elements are local axis of object
					//4th element is local center of object
					//assign to a vector
					//add to a container
				}
			} break;
			default : break;
		}
	}
	void C3DSLoader::readbytes(void* ptr, size_t bytes) {
		if (!stream.read((char *)ptr, bytes))
			finished = true;
	}
	void C3DSLoader::skip() {
		stream.seekg(offset + length);
	}
	std::string C3DSLoader::getstr() {
		char buf = 1; // >0
		std::string str;

		while (buf && !finished) {
			readbytes(&buf,1);
			str.push_back(buf);
		}
		return str;
	}
	void C3DSLoader::skipstr() {
		char buf = 1; // >0

		while (buf && !finished) {
			readbytes(&buf,1);
		}
	}
}
