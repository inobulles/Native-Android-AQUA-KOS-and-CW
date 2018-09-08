
#ifndef __AQUA__ANDROID_KOS_GL_TEXTURE_H
#define __AQUA__ANDROID_KOS_GL_TEXTURE_H

#include "../lib/structs.h"
#include "../../asm/asm.h"

#ifndef TEXTURE_WARNING
#define TEXTURE_WARNING 0
#endif

#define SHARP_TEXTURES 1
#define TEXTURE_WRAP_TYPE GL_MIRRORED_REPEAT // pushes colour to absolute edge, GL_CLAMP_TO_EDGE to wrap colour around

texture_t __texture_create(unsigned long long* data, unsigned long long bpp, unsigned long long width, unsigned long long height, unsigned char warning) {
	if (warning && ((((width & ~(width - 1)) == width) ? 0 : 1) || (((height & ~(height - 1)) == height) ? 0 : 1))) {
		printf("WARNING The width (%lld) and height (%lld) you have provided are not powers of 2\n", width, height);

	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	texture_t texture_id;
	glGenTextures(1, (GLuint*) &texture_id);

	glBindTexture(GL_TEXTURE_2D, (GLuint) texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp == 24 ? GL_RGB : GL_RGBA, (GLuint) width, (GLuint) height, 0, bpp == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TEXTURE_WRAP_TYPE); // x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TEXTURE_WRAP_TYPE); // y axis

#if SHARP_TEXTURES
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#else
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

	return texture_id;

}

texture_t texture_create(unsigned long long* data, unsigned long long bpp, unsigned long long width, unsigned long long height) {
	return __texture_create(data, bpp, width, height, TEXTURE_WARNING);

}

void texture_remove(texture_t __this) {
	glDeleteTextures(1, (GLuint*) &__this);

}

#endif