#include "tga.hpp"

int main() {
	Image img;
	load_tga_image("example.tga", img);
	save_tga_image("example3.tga", img);

	return 0;
}
