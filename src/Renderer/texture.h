#pragma once

#include "external/stb_image.h" // external lib

#include <glad/glad.h>
#include <iostream>
using std::cout; using std::endl;

struct Texture {

	GLuint texture;

	Texture(const char* texturePath, int style, int filter, int rgbParamIn, int rgbParamOut) {

		int width, height, nrChannels;
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, style);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, style);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		if(data){
			glTexImage2D(GL_TEXTURE_2D, 0, rgbParamOut, width, height, 0, rgbParamIn, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			cout << "failed to load texture" << "\ntexture info: " << texturePath << endl;
		}
		stbi_image_free(data);
	}
};