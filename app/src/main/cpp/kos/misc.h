
#ifndef __AQUA__ANDROID_KOS_MISC_H
#define __AQUA__ANDROID_KOS_MISC_H

#include "lib/macros.h"

#define SYS_PRIVILEGES 0

unsigned long long get_platform(void) {
	return PLATFORM_64BIT | PLATFORM_ANDROID | PLATFORM_MOBILE | PLATFORM_KNOWN;

}

unsigned long long platform_system(unsigned long long command) {
	if (SYS_PRIVILEGES) return (unsigned long long) system((const char*) command);
	else printf("WARNING You do not have SYS_PRIVILEGES to run command `%s` (function `platform_system`)\n", (const char*) command);

	return 1;

}

#define DEVICE_NULL     0
#define DEVICE_TEXTURE  1
#define DEVICE_MATH     4

unsigned long long is_device_supported(unsigned long long __device) {
	const char* device = (const char*) __device;

	if (strcmp(device, "math") == 0) return DEVICE_MATH;
	else                             return DEVICE_NULL;

}

#define KOS_DEVICE_COMMAND_WARNING(device_name) ALOGE("WARNING The command you have passed to the " device_name " device (%s) is unrecognized\n", extra);
static unsigned long long previous_math_device_sqrt_result;
#define FLOAT_ONE 1000000

unsigned long long* get_device(unsigned long long device, unsigned long long __extra) {
	const char* extra = (const char*) __extra;
	unsigned long long* result = (unsigned long long*) 0;

	switch (device) {
		case DEVICE_MATH: {
			if (strcmp(extra, "sqrt") == 0) {
				typedef struct {
					char signature[sizeof(uint64_t)];
					uint64_t x;

				} math_device_sqrt_t;

				math_device_sqrt_t* data = (math_device_sqrt_t*) extra;
				previous_math_device_sqrt_result = (unsigned long long) (sqrt((double) data->x / FLOAT_ONE) * FLOAT_ONE);
				result = &previous_math_device_sqrt_result;

			} else {
				KOS_DEVICE_COMMAND_WARNING("math")

			}

			break;

		} case DEVICE_NULL: {
			printf("WARNING The device you have selected is DEVICE_NULL\n");
			break;

		} default: {
			printf("WARNING Device %lld does not seem to exist or doesn't accept `get` commands\n", device);
			break;

		}

	}

	return result;

}

void send_device(unsigned long long device, unsigned long long __extra, unsigned long long __data) {
	const char*         extra = (const char*)         __extra;

	switch (device) {
		case DEVICE_TEXTURE: {
			/*if (strcmp(extra, "sharp") == 0) SHARP_TEXTURES = *data; /// TODO
			else*/ KOS_DEVICE_COMMAND_WARNING("texture")

			break;

		} case DEVICE_NULL: {
			printf("WARNING The device you have selected is DEVICE_NULL\n");
			break;

		} default: {
			printf("WARNING Device %lld does not seem to exist or doesn't accept `send` commands\n", device);
			break;

		}

	}

	unsigned long long* data  = (unsigned long long*) __data;

}

#endif