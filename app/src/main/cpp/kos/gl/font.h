//
// Created by obiwac on 22.07.18.
//

#ifndef ANDROID_FONT_H
#define ANDROID_FONT_H

font_t new_font(const char* _path, unsigned long long size) {
	unsigned long long bytes;
	char* buffer;

	GET_PATH(_path);

	if (load_asset_bytes(path, &buffer, &bytes)) {
		printf("WARNING Font file could not be opened (probably wrong path `%s`)\n", path);

	} else {
		free(buffer);

	}

	printf("JEFF MY nAME IS STILL\n");
	jint error = CALLBACK_INT(java_new_font, callback_env->NewStringUTF(path), (jint) size);

	if (error < 0) {
		printf("WARNING Java had a problem loading the font\n");

	}

	return (font_t) error;

}

unsigned long long get_font_width(font_t font, const char* text) { return (unsigned long long) CALLBACK_INT(java_get_font_width, font, callback_env->NewStringUTF(text)); }
unsigned long long get_font_height(font_t font, const char* text) { return (unsigned long long) CALLBACK_INT(java_get_font_height, font, callback_env->NewStringUTF(text)); }

void font_remove(font_t font) {
	CALLBACK_VOID(java_font_remove, font);

}

texture_t create_texture_from_font(font_t font, const char* text) {
	jbyteArray error = (jbyteArray) CALLBACK(java_create_texture_from_font, callback_env->CallStaticObjectMethod, font, callback_env->NewStringUTF(text));

	printf("%p\n", error);

	if (error == NULL) {
		printf("WARNING Java had a problem loading the font\n");

	}

	return (texture_t) texture_create((unsigned long long*) error, get_font_width(font, text), get_font_height(font, text), 32);

}

#endif
