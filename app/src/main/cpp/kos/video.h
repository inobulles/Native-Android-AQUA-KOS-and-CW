
#ifndef __AQUA__ANDROID_KOS_VIDEO_H
#define __AQUA__ANDROID_KOS_VIDEO_H

#include "lib/structs.h"
#include "gl/surface.h"
#include "gl/texture.h"

unsigned char waiting_video_flip = 0;

unsigned long long gl_width = 0;
unsigned long long gl_height = 0;

unsigned long long gl_resize = 0;
unsigned long long gl_fps = 0;

unsigned long long video_fps(void) {
	return gl_fps;

}

void video_clear(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void video_clear_colour(unsigned long long red, unsigned long long green, unsigned long long blue, unsigned long long alpha) {
	glClearColor(
		(GLfloat) red / (GLfloat) _UI64_MAX,
		(GLfloat) green / (GLfloat) _UI64_MAX,
		(GLfloat) blue / (GLfloat) _UI64_MAX,
		(GLfloat) alpha / (GLfloat) _UI64_MAX
	);

}

void video_draw(void) {
	printf("WARNING Deprecated function `video_draw`\n");

}

void video_flip(void) {
	waiting_video_flip = 1;

}

unsigned long long video_width(void) { return gl_width; }
unsigned long long video_height(void) { return gl_height; }

void set_video_visibility(signed long long state) {
	printf("WARNING This platform does not support this function `set_video_visibility`\n");

}

void* new_rectangle(unsigned long long x, unsigned long long y, unsigned long long width, unsigned long long height) { printf("WARNING Deprecated function `new_rectangle`\n"); return nullptr; }
void draw_rectangle(void* __this) { printf("WARNING Deprecated function `draw_rectangle`\n"); }

#endif