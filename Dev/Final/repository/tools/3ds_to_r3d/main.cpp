#include <std/assert>
#include <std/io>

#include "blas.hpp"

#include "3ds.hpp"
#include "r3d.hpp"

#include <string>

void calculate_normals(const float* points, float* normals, size_t n_points) {
	for (size_t i = 0; i < n_points; i += 3) {
		const float x = points[i];
		const float y = points[i + 1];
		const float z = points[i + 2];

		const Vector3 normal = Vector3(x, y, z).normal();

		normals[i] = normal.x;
		normals[i+1] = normal.y;
		normals[i+2] = normal.z;
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Too few arguments\n");
		return 0;
	}

	const char* import_fn(argv[1]);
	std::string export_fn(import_fn);
	size_t idx = export_fn.find_last_of(".");
	if (idx != std::string::npos) export_fn.erase(idx);
	export_fn.append(".r3d");

	Model orig;
	load_3ds_model(import_fn, orig);

	printf("Importing model from %s\n", import_fn);
	size_t n_meshes = orig.meshes.size();
	//printf("n Meshes: %li\n", n_meshes);
	//printf("Calculating Normals...\n");
	for (size_t i = 0; i < n_meshes; ++i) {
		Mesh& mesh = orig.meshes[i];

		//printf("Mesh[%li] n Tris: %li\n", i, mesh.indices.size() / 3);
		//printf("Mesh[%li] n Verts: %li\n", i, mesh.points.size() / 3);

		mesh.normals.resize(mesh.points.size());
		calculate_normals(&mesh.points[0], &mesh.normals[0], mesh.points.size());
	}

	printf("Exporting model to %s\n", export_fn.c_str());
	save_r3d_model(export_fn.c_str(), orig);

	printf("Validating...\n");
	Model test;
	load_r3d_model(export_fn.c_str(), test);

	assert(test.meshes.size() == orig.meshes.size());
	for (size_t i = 0; i < n_meshes; ++i) {
		const Mesh& omesh = orig.meshes[i];
		const Mesh& tmesh = test.meshes[i];

		assert(omesh.indices == tmesh.indices);
		assert(omesh.points == tmesh.points);
		assert(omesh.normals == tmesh.normals);
		assert(omesh.uvcoords == tmesh.uvcoords);
	}
	printf("Conversion Completed\n");

	return 0;
}
