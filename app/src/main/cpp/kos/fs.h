
#ifndef __AQUA__ANDROID_KOS_FS_H
#define __AQUA__ANDROID_KOS_FS_H

#define FS_SUPPORT_CHECK \
	if (!fs_support()) { \
		printf("WARNING FS is not supported on this platform\n"); \
		\
	} else

unsigned long long fs_support(void) {
	return 1; /// TODO set to 1

}

unsigned long long fs_read(unsigned long long _path, unsigned long long data, unsigned long long length) {
	FS_SUPPORT_CHECK {
		GET_PATH(_path)

		if (load_asset_bytes(path, (char**) data, (unsigned long long*) length)) {
//			*data   = (char*) 0;
//			*length = 0;

			ALOGE("WARNING File `%s` could not be opened (for reading)\n", path);
			return 1;

		}

		return 0;

	}

	return 1;

}

void fs_free(unsigned long long data, unsigned long long length) {
	FS_SUPPORT_CHECK {
		free((void*) data);

	}

}

unsigned long long fs_write(unsigned long long _path, unsigned long long data, unsigned long long length) {
	FS_SUPPORT_CHECK {
		extern bool default_assets;

		if (!default_assets) {
			char* final_path;
			GET_PATH(_path)
			SET_FINAL_PATH

			FILE* file = fopen(final_path, "wb");
			free(              final_path);

			if (!file) {
				ALOGE("WARNING File `%s` could not be opened (for writing)\n", path);
				return 1;

			}

			fwrite((const void*) data, sizeof(char), length, file);
			fclose(file);

			return 0;

		} else {
			ALOGE("WARNING `default_assets` is set\n");
			return 1;

		}

	}

	return 1;

}

#endif