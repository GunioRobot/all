#include <std/io>
#include <std/types>

#include "3ds.hpp"

#define MAIN3DS	0x4D4D
	#define EDIT3DS	0x3D3D
		#define EDIT_MATERIAL	0xAFFF
			#define MAT_NAME	0xA000
			#define MAT_AMBIENT	0xA010
			#define MAT_DIFFUSE	0xA020
			#define MAT_SPECULAR	0xA030
			#define MAT_TEXTURE	0xA200
			#define MAT_TEXFILE	0xA300
			#define MAT_VCOORDS	0xA354
			#define MAT_UCOORDS	0xA356
			#define MAT_UOFFSET	0xA358
			#define MAT_VOFFSET	0xA35A
		#define EDIT_OBJECT	0x4000
			#define OBJ_TRIMESH	0x4100
				#define TRI_VERTEXL	0x4110
				#define TRI_FACEL1	0x4120
					#define TRI_FACE_MATERIAL	0x4130
					#define TRI_FACE_SMOOTHING	0x4150
				#define TRI_TEXCOORD	0x4140
				#define TRI_LOCAL	0x4160

void load_3ds_model(const char* file_name, Model& model) {
	File file(file_name, "rb");
	assert(file.open());

	uint id = 0;
	size_t length = 0, offset = 0;

	while ((offset = file.offset()) < file.length()) {
		file.read(&id, 2);
		file.read(&length, 4);

		switch (id) {
			case MAIN3DS : break;
			case EDIT3DS : break;
			case EDIT_OBJECT : {
				char buf;
				do { // skip string
					file.read(&buf, 1);
				} while (buf != '\0');
			} break;
			case OBJ_TRIMESH : {
				model.meshes.push_back(Mesh());
			} break;
			case TRI_VERTEXL : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, 2);
				count *= 3;
				mesh.points.resize(count);
				file.read(&mesh.points[0], count * sizeof(mesh.points[0]));
			} break;
			case TRI_FACEL1 : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, 2);
				count *= 3;
				mesh.indices.resize(count);

				byte buffer[2];
				for (uint i = 0; i < count; i += 3) {
					file.read(&mesh.indices[i], 6);
					file.read(&buffer, 2); // useless
				}
			} break;
			case TRI_TEXCOORD : {
				Mesh& mesh = model.meshes.back();

				size_t count = 0;
				file.read(&count, 2);
				count *= 2;
				mesh.uvcoords.resize(count);
				file.read(&mesh.uvcoords[0], count * sizeof(mesh.uvcoords[0]));
			} break;
			default : { file.seek(offset + length); } // skip block
		}
	}
}
