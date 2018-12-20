
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FS_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FS_H
	
	#include "../macros_and_inclusions.h"
	
	unsigned long long fs_support(void) {
		return 1;
		
	}
	
	#define FS_CHECK_FILE(action) \
		if (!file) { \
			printf("WARNING File `%s` could not be opened (for %s)\n", path, action); \
			return 1; \
		}
	
	static unsigned long long __fs_read(unsigned long long _path, unsigned long long data, unsigned long long bytes, unsigned long long offset) {
		GET_PATH((char*) _path);
		
		FILE* file = fopen(path, "rb");
		FS_CHECK_FILE("reading")
		
		fseek(file, 0, SEEK_END);
		*((unsigned long long*) bytes) = ftell(file) + offset;
		rewind(file);
		
		*((char**) data) = (char*) malloc(*((unsigned long long*) bytes) + 1);
		fread(*((char**) data) + offset,  *((unsigned long long*) bytes) - offset, sizeof(char), file);
		
		fclose(file);
		return 0;
		
	}
	
	unsigned long long fs_read(unsigned long long _path, unsigned long long data, unsigned long long bytes) {
		return       __fs_read(                   _path,                    data,                    bytes, 0);
		
	}
	
	void fs_free(unsigned long long data, unsigned long long bytes) {
		free((char*) data);
		
	}
	
	unsigned long long fs_write(unsigned long long _path, unsigned long long data, unsigned long long bytes) {
		GET_PATH((char*) _path);
		
		FILE* file = fopen(path, "wb");
		FS_CHECK_FILE("writing")
		
		fwrite((const void*) ((const char*) data), sizeof(char), bytes, file);
		fclose(file);
		
		return 0;
		
	}
	
	#include "fs_list.h"
	
#endif
