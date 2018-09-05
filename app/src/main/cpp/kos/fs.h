
#ifndef __AQUA__ANDROID_KOS_FS_H
#define __AQUA__ANDROID_KOS_FS_H

#define FS_SUPPORT_CHECK \
	if (!fs_support()) { \
		printf("WARNING FS is not supported on this platform\n"); \
		 \
	} else

unsigned long long fs_support(void) {
	return 0;

}

unsigned long long fs_read(const char* path, char** data, unsigned long long* length) {
	FS_SUPPORT_CHECK {
		printf("TODO `fs_read`\n");
		return 0;

	}

	return 1;

}

void fs_free(char** data, unsigned long long* length) {
	FS_SUPPORT_CHECK {
		printf("TODO `fs_free`\n");

	}

}

unsigned long long fs_write(const char* path, const char* data, unsigned long long length) {
	FS_SUPPORT_CHECK {
		printf("TODO `fs_write`\n");
		return 0;

	}

	return 1;

}

#endif