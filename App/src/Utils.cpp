#include "../include/Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <string>

#include "../include/stb_image.hpp"

unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels) {
	return stbi_load(path.c_str(), &width, &height, &nrChannels, desiredChannels);
}

void flipImagesVertically(const bool &val) {
	stbi_set_flip_vertically_on_load(val);
}

void freeImageData(void *data) {
	stbi_image_free(data);
}
