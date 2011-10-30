#include "tga.hpp"

#include <std/assert>
#include <std/io>
#include <std/types>

#include <string.h>

void load_tga_image(const char* file_name, Image& img) {
	File file(file_name, "rb");
	assert(file.open());

	// TARGA header (skip unused info)
	ushort type;
	byte buffer[8];
	file.read(buffer, 2);
	file.read(&type, 2);
	file.read(buffer, 8);
	file.read(&img.width, 2);
	file.read(&img.height, 2);
	file.read(&img.bitdepth, 1);
	file.read(buffer, 1);

	// Some useful constants
	const ubyte bytes_per_pixel = img.bitdepth / 8;
	const uint n_pixels = img.width * img.height;
	const uint n_bytes = n_pixels * bytes_per_pixel;

	// pre-allocate
	img.data.resize(n_bytes);

	if (type == 2 || type == 3) {
		file.read(&img.data[0], n_bytes);
	} else if (type & 8) { // Compressed
		uint bytes_read = 0;
		do {
			ubyte header;
			file.read(&header, 1);

			const ubyte run_length = (ubyte)((header & 127) + 1); // bit mask, +1 for 128 max
			const uint byte_length = run_length * bytes_per_pixel;

			ubyte pixel[4] = {0}; // 32bit pixel buffer

			if (header & 128) { // RLE
				file.read(&pixel[0], bytes_per_pixel);

				for (uint i = 0; i < byte_length; i += bytes_per_pixel) {
					for (uint j = 0; j < bytes_per_pixel; ++j) {
						img.data[bytes_read + (i + j)] = pixel[j];
					}
				}
			} else { // RAW
				for (uint i = 0; i < byte_length; i += bytes_per_pixel) {
					file.read(&pixel[0], bytes_per_pixel);

					for (uint j = 0; j < bytes_per_pixel; ++j) {
						img.data[bytes_read + (i + j)] = pixel[j];
					}
				}
			}
			bytes_read += byte_length;
		} while (bytes_read < n_bytes);
	}
}

void save_tga_image(const char* file_name, const Image& img) {
	File file(file_name, "wb");
	assert(file.open());

	ushort type = 0;
	switch (img.bitdepth) {
		case 8 : { type = 3; } break; // GRAYSCALE
		case 24 : { type = 2; } break; // RGB
		case 32 : { type = 2; } break; // RGBA
	}

	// TARGA header
	const byte buffer[8] = {0};
	file.write(buffer, 2);
	file.write(&type, 2);
	file.write(buffer, 8);
	file.write(&img.width, 2);
	file.write(&img.height, 2);
	file.write(&img.bitdepth, 1);
	file.write(buffer, 1);

	file.write(&img.data[0], img.data.size());
}
