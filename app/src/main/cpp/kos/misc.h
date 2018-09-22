
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

#define DEVICE_NULL    0
#define DEVICE_TEXTURE 1
#define DEVICE_MATH    4
#define DEVICE_CLOCK   5
#define DEVICE_ANDROID 6

unsigned long long is_device_supported(unsigned long long __device) {
	const char* device = (const char*) __device;

	if      (strcmp(device, "math")    == 0) return DEVICE_MATH;
	else if (strcmp(device, "texture") == 0) return DEVICE_TEXTURE;
	else if (strcmp(device, "clock")   == 0) return DEVICE_CLOCK;
	else if (strcmp(device, "android") == 0) return DEVICE_ANDROID;
	else                                     return DEVICE_NULL;

}

typedef struct {
	uint64_t hour;
	uint64_t minute;
	uint64_t second;

	uint64_t day;
	uint64_t month;
	uint64_t year;

	uint64_t week_day;
	uint64_t year_day;

} time_device_t;

static struct tm*    kos_tm_struct = (struct tm*) 0;
static time_t        kos_time      = 0;
static time_device_t previous_time_device;

static jint previous_package_existance;

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

		} case DEVICE_CLOCK: {
			kos_time = time(NULL);
			kos_tm_struct = localtime(&kos_time);

			if (strcmp(extra, "current") == 0) {
				previous_time_device.hour     = (uint64_t) kos_tm_struct->tm_hour;
				previous_time_device.minute   = (uint64_t) kos_tm_struct->tm_min;
				previous_time_device.second   = (uint64_t) kos_tm_struct->tm_sec;

				previous_time_device.day      = (uint64_t) kos_tm_struct->tm_mday;
				previous_time_device.month    = (uint64_t) kos_tm_struct->tm_mon;
				previous_time_device.year     = (uint64_t) kos_tm_struct->tm_year;

				previous_time_device.week_day = (uint64_t) kos_tm_struct->tm_wday;
				previous_time_device.year_day = (uint64_t) kos_tm_struct->tm_yday;

				result = (unsigned long long*) &previous_time_device;

			} else {
				KOS_DEVICE_COMMAND_WARNING("clock")

			}

			break;

		} case DEVICE_ANDROID: {
			if (extra[0] == 'p' && extra[1] == 'k' && extra[2] == 'g' && extra[3] == 'e') {
				extra += 4;
				previous_package_existance = CALLBACK_INT(java_package_exists, callback_env->NewStringUTF(extra));
				result = (unsigned long long*) &previous_package_existance;

			} else {
				KOS_DEVICE_COMMAND_WARNING("android")

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
	unsigned long long* data  = (unsigned long long*) __data;

	switch (device) {
		case DEVICE_TEXTURE: {
			if (strcmp(extra, "sharp") == 0) SHARP_TEXTURES = *data;
			else KOS_DEVICE_COMMAND_WARNING("texture")

			break;

		} case DEVICE_ANDROID: {
			if (strcmp(extra, "package open") == 0) CALLBACK_VOID(java_package_open, callback_env->NewStringUTF((const char*) data));
			else KOS_DEVICE_COMMAND_WARNING("android");

			break;

		} case DEVICE_NULL: {
			printf("WARNING The device you have selected is DEVICE_NULL\n");
			break;

		} default: {
			printf("WARNING Device %lld does not seem to exist or doesn't accept `send` commands\n", device);
			break;

		}

	}

}

#endif