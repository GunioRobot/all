#include <boost/shared_ptr.hpp>
#include "CMesh.h"

CMesh_ptr createRectangle(double w, double h, double l) {
	const int n_vertices = 8 * 3, n_indices = 6 * 3, n_texcoords = 8 * 2;
	
	CMesh_ptr mesh(new CMesh());

	mesh->vertices.resize(n_vertices);
	float xm = -0.5, ym = -0.5, zm = 0.5;
	for (int i = 0; i < n_vertices; i+=3) {
		if (i == 3) ym = -ym;
		if (i == 6) xm = -xm;
		if (i == 9) ym = -ym;
		if (i == 12) zm = -zm;
		
		if (i == 15) ym = -ym;
		if (i == 18) xm = -xm;
		if (i == 21) ym = -ym;
		
		std::cout<< xm * 1 << '\n';
		std::cout<< ym * 1 << '\n';
		std::cout<< zm * 1 << '\n';
		std::cout<< '\n';
	}

	
	mesh->indices.resize(n_indices);
	for (unsigned int i = 0; i < n_indices; i += 3) {
		unsigned short a = 0, b = 0, c = 0, f = 0;

		mesh->indices[i] = a;
		mesh->indices[i+1] = b;
		mesh->indices[i+2] = c;
		//mesh->indices[i+3] = f; //unused...
	}
	
	return mesh;
} 
