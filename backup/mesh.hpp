#pragma once

#include <std/types>

struct Mesh {
	vector<ushort> indices;
	vector<float> points, normals, uvcoords;
};
