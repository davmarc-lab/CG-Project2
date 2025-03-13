#include "../include/Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <string>

#include "../include/stb_image.hpp"

unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels) {
	return stbi_load(path.c_str(), &width, &height, &nrChannels, desiredChannels);
}

void freeImageData(void *data) {
	stbi_image_free(data);
}
