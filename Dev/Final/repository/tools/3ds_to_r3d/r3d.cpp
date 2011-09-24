#include <std/io>
#include <std/types>

#include "r3d.hpp"

#define MESH	0x3000
	#define INDICES		0x4001
	#define	POINTS		0x4002
	#define NORMALS		0x4003
	#define UVCOORDS	0x4004

void load_r3d_model(const char* file_name, Model& model) {
	File file(file_name, "rb");
	assert(file.open());

	size_t id = 0;
	size_t length = 0, offset = 0;
	size_t file_length = file.length();

	while ((offset = file.offset()) < file_length) {
		file.read(&id, sizeof(id));
		file.read(&length, sizeof(length));

		switch (id) {
			case MESH : {
				model.meshes.push_back(Mesh());
			} break;
			case INDICES : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, sizeof(count));
				mesh.indices.resize(count);
				file.read(&mesh.indices[0], count * sizeof(mesh.indices[0]));
			} break;
			case POINTS : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, sizeof(count));
				mesh.points.resize(count);
				file.read(&mesh.points[0], count * sizeof(mesh.points[0]));
			} break;
			case NORMALS : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, sizeof(count));
				mesh.normals.resize(count);
				file.read(&mesh.normals[0], count * sizeof(mesh.normals[0]));
			} break;
			case UVCOORDS : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, sizeof(count));
				mesh.uvcoords.resize(count);
				file.read(&mesh.uvcoords[0], count * sizeof(mesh.uvcoords[0]));
			} break;
			default : { file.seek(offset + length); } // skip block
		}
	}
}

namespace {
	void save_id(File& file, size_t id, size_t length) {
		file.write(&id, sizeof(id));
		file.write(&length, sizeof(length));
	}

	void save_chunk(File& file, size_t id, const void* source, size_t count, size_t size) {
		if (count && source) {
			const size_t length = count * size;

			save_id(file, id, sizeof(count) + length);
			file.write(&count, sizeof(count));
			file.write(source, length);
		}
	}
}

void save_r3d_model(const char* file_name, const Model& model) {
	File file(file_name, "wb");
	assert(file.open());

	const size_t n_meshes = model.meshes.size();
	for (size_t i = 0; i < n_meshes; ++i) {
		const Mesh& mesh = model.meshes[i];

		save_id(file, MESH, 0); // TODO - Calculate mesh size
		save_chunk(file, INDICES, &mesh.indices[0], mesh.indices.size(), sizeof(mesh.indices[0]));
		save_chunk(file, POINTS, &mesh.points[0], mesh.points.size(), sizeof(mesh.points[0]));
		save_chunk(file, NORMALS, &mesh.normals[0], mesh.normals.size(), sizeof(mesh.normals[0]));
		save_chunk(file, UVCOORDS, &mesh.uvcoords[0], mesh.uvcoords.size(), sizeof(mesh.uvcoords[0]));
	}
}
