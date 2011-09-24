#pragma once

#include <std/types>

struct Image {
	uint width, height;
	ubyte bitdepth;
	vector<ubyte> data;

	Image() : width(0), height(0), bitdepth(0), data(width * height) {}
};
