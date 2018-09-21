//
// Created by obiwac on 21.09.18.
//

#ifndef ANDROID_SAFEMEM_H
#define ANDROID_SAFEMEM_H

#include <stdlib.h>
#include <vector>

static inline void* safe_malloc(unsigned long long bytes) {
	void* pointer = malloc(bytes);

	if (!pointer) {
		ALOGE("WARNING Could not allocate %lld bytes (malloc)\n", bytes);

	}

	return pointer;

}

static inline void safe_free(void* pointer) {
	if (!pointer) {
		ALOGE("WARNING Trying to free nullptr. Aborting ...\n");

	} else {
		free(pointer);

	}

}

#define malloc safe_malloc
#define free   safe_free

#endif //ANDROID_SAFEMEM_H
