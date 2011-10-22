#pragma once

#include <std/types>

struct Image {
	uint width, height;
	ubyte bitdepth;
	vector<ubyte> data;
};
