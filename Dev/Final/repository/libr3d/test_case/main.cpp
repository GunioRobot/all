#include "r3d.hpp"

#include <std/io>

int main() {
	Model model;
	load_r3d_model("example.r3d", model);

	printf("n Meshes: %li\n", model.meshes.size());
	for (size_t i = 0; i < model.meshes.size(); ++i) {
		const Mesh& mesh = model.meshes[i];

		printf("Mesh[%li] n Tris: %li\n", i, mesh.indices.size() / 3);
		printf("Mesh[%li] n Verts: %li\n", i, mesh.points.size() / 3);
	}

	return 0;
}
