//
// Created by obiwac on 17.09.18.
//

#ifndef ANDROID_SCREENSHOT_H
#define ANDROID_SCREENSHOT_H

#define CREATE_TEXTURE_FROM_SCREENSHOT_FUNCTION 1

texture_t create_texture_from_screenshot(void) {
	unsigned long long bpp   = 32;
	unsigned long long bytes = video_width() * video_height() * (bpp / 8);

	unsigned char* pixels = (unsigned char*) malloc(bytes);
	//glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, (GLsizei) video_width(), (GLsizei) video_height(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	texture_t texture = texture_create((unsigned long long) pixels, bpp, video_width(), video_height());
	free(pixels);

	if (texture == -1ull) {
		ALOGE("WARNING Could not create texture from screenshot\n");

	}

	return texture;

}

#endif //ANDROID_SCREENSHOT_H
