
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_DECODERS_BMP_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_DECODERS_BMP_H
	
	#include "../../macros_and_inclusions.h"
	
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
	
	void bmp_load(bitmap_image_t* this, unsigned long long _path) {
		this->data = (void*) 0;
		
		char* buffer;
		GET_PATH((char*) _path);
		
		FILE* file = fopen(path, "rb");
		
		if (!file) {
			printf("WARNING Image file could not be opened (probably wrong path `%s`)\n", path);
			return;
			
		}
		
		bitmap_header_t header;
		bitmap_info_header_t info_header;
		
		fread((char*) &header, sizeof(bitmap_header_t), 1, file);
		
		if (header.magic != 0x4D42) {
			printf("WARNING File is not a bitmap image\n");
			fclose(file);
			
			return;
			
		}
		
		fread((char*) &info_header, sizeof(bitmap_info_header_t), 1, file);
		
		this->image_size = info_header.image_bytes / sizeof(unsigned long long);
		this->width  = info_header.width;
		this->height = info_header.height;
		
		unsigned char* char_data = (unsigned char*) malloc(info_header.image_bytes);
		unsigned char temp;
		
		fseek(file, header.offset, SEEK_SET);
		fread(char_data, info_header.image_bytes, 1, file);
		this->bpp = info_header.bpp;
		
		int i;
		for (i = 0; i < info_header.image_bytes; i += this->bpp / 8) {
			if (this->bpp == 32) {
				unsigned char a = char_data[i];
				unsigned char r = char_data[i + 1];
				unsigned char g = char_data[i + 2];
				unsigned char b = char_data[i + 3];
				
				char_data[i]     = b;
				char_data[i + 1] = g;
				char_data[i + 2] = r;
				char_data[i + 3] = a;
				
			} else {
				temp             = char_data[i];
				char_data[i]     = char_data[i + 2];
				char_data[i + 2] = temp;
				
			}
			
		}
		
		this->data               = (unsigned long long*) malloc(info_header.image_bytes);
		unsigned char* data8     = (unsigned char*)      this->data;
		unsigned long long pitch = (unsigned long long)  info_header.width * (info_header.bpp / 8);
		
		int y;
		for (y = 0; y < info_header.height; y++) {
			memcpy(data8 + (info_header.height - y - 1) * pitch, char_data + y * pitch, pitch);
			
		}
		
		free(char_data);
		fclose(file);
	
	}
	
	void bmp_free(bitmap_image_t* this) {
		free(this->data/*, this->image_size * sizeof(unsigned long long)*/);
	
	}
	
#endif
