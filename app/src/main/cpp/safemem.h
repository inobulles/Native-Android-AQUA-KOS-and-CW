//
// Created by obiwac on 21.09.18.
//

#ifndef ANDROID_SAFEMEM_H
#define ANDROID_SAFEMEM_H

#include <stdlib.h>
#include <vector>

#define CPP_MMAN_STYLE true

static inline void* safe_malloc(unsigned long long bytes) {
#if CPP_MMAN_STYLE
	char* pointer =  new char[bytes];
#else
	void* pointer = malloc(bytes);

	if (!pointer) {
		ALOGE("WARNING Could not allocate %lld bytes (malloc)\n", bytes);

	}
#endif

	return (void*) pointer;

}

static inline void safe_free(void* pointer) {
#if CPP_MMAN_STYLE
	delete[] (char*) pointer;
#else
	if (!pointer) {
		ALOGE("WARNING Trying to free nullptr. Aborting ...\n");

	} else {
		free(pointer);

	}
#endif

}

#define malloc safe_malloc
#define free   safe_free

#endif //ANDROID_SAFEMEM_H
