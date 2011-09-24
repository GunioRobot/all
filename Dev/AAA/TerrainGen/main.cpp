#include <cstdlib>
#include <io/tga.hpp>
#include <iostream>
#include <vector>

typedef std::vector<signed char> field_row;
typedef std::vector<field_row> field;

signed char scrand(signed char r = 127) { return (-r + 2 * (std::rand() % r)); }

field mdp(const field& base, signed char r) {
	size_t n = (2 * base.size()) - 1;

	field copy(n, field_row);

	// Resize
	// 1 0 1
	// 0 0 0
	// 1 0 1
	for (size_t i = 0; i < n; i += 2) {
		for (size_t j = !(i % 2 == 0); j < n; j += 2) {
			copy[i][j] = base[i / 2][j / 2];
		}
	}

	// Diamond algorithm
	// 0 0 0
	// 0 1 0
	// 0 0 0
	for (size_t i = 1; i < n; i += 2) {
		for (size_t j = 1; j < n; j += 2) {
			signed char& map_ij = copy[i][j];

			signed char a = copy[i - 1][j - 1];
			signed char b = copy[i - 1][j + 1];
			signed char c = copy[i + 1][j - 1];
			signed char d = copy[i + 1][j + 1];
			map_ij = (a + b + c + d) / 4;

			signed char rv = scrand(r);
			if (map_ij + rv < 127 && map_ij + rv > -128) map_ij += rv;
		}
	}

	// Square algorithm
	// 0 1 0
	// 1 0 1
	// 0 1 0
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = (i % 2 == 0); j < n; j += 2) {
			signed char& map_ij = copy[i][j];

			// get surrounding values
			signed char a = 0, b = a, c = a, d = a;
			if (i != 0) a = copy[i - 1][j];
			if (j != 0) b = copy[i][j - 1];
			if (j + 1 != n) c = copy[i][j + 1];
			if (i + 1 != n) d = copy[i + 1][j];

			// average calculation
			if (i == 0) map_ij = (b + c + d) / 3;
			else if (j == 0) map_ij = (a + c + d) / 3;
			else if (j + 1 == n) map_ij = (a + b + d) / 3;
			else if (i + 1 == n) map_ij = (a + b + c) / 3;
			else map_ij = (a + b + c + d) / 4;

			signed char rv = scrand(r);
			if (map_ij + rv < 127 && map_ij + rv > -128) map_ij += rv;
		}

	}
	return copy;
}

field createnoise(unsigned n) {
	field noise(n, field_row);
	for (unsigned i = 0; i < n; ++i) {
		for (unsigned j = 0; j < n; ++j) noise[i][j] = scrand();
	}
	return noise;
}

void smoothen(field base) {
	for (unsigned i = 1; i < base.size() - 1; ++i) {
		for (unsigned j = 1; j < base.size() - 1; ++j) {
			base[i][j] = (
				base[i - 1][j - 1] +	base[i - 1][j] +	base[i - 1][j + 1] +
				base[i + 0][j - 1] +	base[i + 0][j] +	base[i + 0][j + 1] +
				base[i + 1][j + 1] +	base[i + 1][j] +	base[i + 1][j - 1]
			) / 9;
		}
	}
}

int main() {
	/////// Heightmap generation
	const unsigned n = 7500; // lowest n^2
	signed char r = 127;

	field final = createnoise(2);
	while (final.size() < n && r > 0) {
		final = mdp(final, r);
		r -= 10;
	}

	//for (int i = 0; i < 15; ++i) smoothen(final);

	// Dump heightmap to TARGA image
	{
		const unsigned m = final.size();

		unsigned char* imgdta = new unsigned char[m * m];
		for (size_t i = 0; i < m; ++i) {
			for (size_t j = 0; j < m; ++j) {
				unsigned char& imgdta_ij = imgdta[(i * m) + j];
				imgdta_ij = 128 + final[i][j];
			}
		}
		char file_name[] = "x_iter.tga";
		tga_image mytga = {m, m, 8, imgdta};
		save_tga_image(file_name, mytga);
		delete[] imgdta;
	}

	return 0;
}
