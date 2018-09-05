//
// Created by obiwac on 05.09.18.
//

#ifndef ANDROID_PREDEFINED_TEXTURES_H
#define ANDROID_PREDEFINED_TEXTURES_H

#include <android/log.h>
#include "lib/structs.h"
#include "gl/texture.h"
#include "../alog.h"
#include "../native-lib.h"

static texture_t predefined_background_texture;
static texture_t predefined_frost_background_texture;

void bmp_load(bitmap_image_t* __this, unsigned long long _path);
void bmp_free(bitmap_image_t* __this);

static int kos_setup_predefined_textures(void) {
	int warning = 0;
	bitmap_image_t temp_bmp;

	bmp_load(&temp_bmp, (unsigned long long) "wallpaper/wallpaper.bmp");
	predefined_background_texture = texture_create(temp_bmp.data, temp_bmp.bpp, temp_bmp.width, temp_bmp.height);
	bmp_free(&temp_bmp);
	warning += predefined_background_texture == -1;

	bmp_load(&temp_bmp, (unsigned long long) "wallpaper/frost.bmp");
	predefined_frost_background_texture = texture_create(temp_bmp.data, temp_bmp.bpp, temp_bmp.width, temp_bmp.height);
	bmp_free(&temp_bmp);
	warning += predefined_frost_background_texture == -1;

	return warning;

}

static void kos_free_predefined_textures(void) {
	if (predefined_background_texture       != -1) texture_remove(predefined_background_texture);
	if (predefined_frost_background_texture != -1) texture_remove(predefined_frost_background_texture);

}

texture_t get_predefined_texture(unsigned long long name) {
	switch (name) {
		case TEXTURE_BACKGROUND:         return predefined_background_texture;
		case TEXTURE_FROSTED_BACKGROUND: return predefined_frost_background_texture;

		default: {
			ALOGW("WARNING Texture %lld is unknown. Returning -1 ...\n", name);
			return (texture_t) -1;

		}

	}

}

void update_predefined_texture(unsigned long long name) {
	switch (name) {
		case TEXTURE_BACKGROUND: {
			break;

		} case TEXTURE_FROSTED_BACKGROUND: {
			break;

		} default: {
			ALOGW("WARNING Texture %lld is unknown\n", name);
			break;

		}

	}

}

#endif //ANDROID_PREDEFINED_TEXTURES_H
