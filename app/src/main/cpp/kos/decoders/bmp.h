//
// Created by obiwac on 19.07.18.
//

#ifndef ANDROID_BMP_H
#define ANDROID_BMP_H

#include <stdint.h>
#include <string.h>
#include "../lib/structs.h"
#include "../../native-lib.h"
#include "../../asm/asm.h"

#pragma pack(push, 1)

typedef struct {
	uint16_t magic;
	uint32_t file_size;

	uint16_t reserved_1;
	uint16_t reserved_2;

	uint32_t offset;

} bitmap_header_t;

typedef struct {
	uint32_t size;

	int32_t width;
	int32_t height;

	uint16_t planes;
	uint16_t bpp;

	uint32_t compression_type;
	uint32_t image_bytes;

	int64_t pixels_per_meter_x;
	int64_t pixels_per_meter_y;

	uint32_t colour_count;
	uint32_t important_colours;

} bitmap_info_header_t;

#pragma pack(pop)

unsigned long long bmp_support(void) {
	return 1;

}

#define BMP_MAGIC 0x4D42

void bmp_load(bitmap_image_t* __this, unsigned long long _path) {
	unsigned long long bytes;

	char* buffer;
	char* original;

	char path[MAX_PATH_LENGTH] = "root/";
	strncat(path, (char*) _path, MAX_PATH_LENGTH - strlen(path));

	if (load_asset_bytes(path, &buffer, &bytes)) {
		printf("WARNING Image file could not be opened (probably wrong path `%s`)\n", path);
		return;

	} else {
		original = buffer;

	}

	bitmap_header_t header;
	bitmap_info_header_t info_header;

	header = *((bitmap_header_t*) buffer);
	buffer += sizeof(bitmap_header_t);

	if (header.magic != BMP_MAGIC) {
		printf("WARNING File is not valid bitmap image\n");
		mfree(original, bytes);
		return;

	}

	info_header = *((bitmap_info_header_t*) buffer);
//	buffer += sizeof(bitmap_info_header_t);

	__this->image_size = info_header.image_bytes / sizeof(unsigned long long);
	__this->bpp = (unsigned long long) info_header.bpp;

	__this->width = (unsigned long long) info_header.width;
	__this->height = (unsigned long long) info_header.height;

	unsigned char* char_data = (unsigned char*) malloc(info_header.image_bytes);
	unsigned char temp;

	buffer = original;
	buffer += header.offset;

	memcpy(char_data, buffer, info_header.image_bytes);

	int i;
	for (i = 0; i < info_header.image_bytes; i += __this->bpp / 8) {
		if (__this->bpp == 32) {
			unsigned char a = char_data[i];
			unsigned char r = char_data[i + 1];
			unsigned char g = char_data[i + 2];
			unsigned char b = char_data[i + 3];

			char_data[i] = b;
			char_data[i + 1] = g;
			char_data[i + 2] = r;
			char_data[i + 3] = a;

		} else {
			temp = char_data[i];
			char_data[i] = char_data[i + 2];
			char_data[i + 2] = temp;

			printf("%d %d %d\n", buffer[i], buffer[i + 1], buffer[i + 2]);

		}

	}

	__this->data = (unsigned long long*) char_data;
	mfree(original, bytes);

}

void bmp_free(bitmap_image_t* __this) {
	mfree(__this->data, __this->image_size * sizeof(unsigned long long));

}

#endif
