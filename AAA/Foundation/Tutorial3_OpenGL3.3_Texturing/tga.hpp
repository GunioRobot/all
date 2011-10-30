#pragma once

#include "image.hpp"

void load_tga_image(const char* file_name, Image& img);
void save_tga_image(const char* file_name, const Image& img);
