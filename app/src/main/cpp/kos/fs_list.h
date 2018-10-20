//
// Created by obiwac on 20.10.18.
//

#ifndef ANDROID_FS_LIST_H
#define ANDROID_FS_LIST_H

#include <dirent.h>
#include "../native-lib.h"

//#define FS_LIST_FUNCTIONS 1
#define FS_LIST_D_NAME_VALID (strcmp(directory->d_name, "..") != 0 && strcmp(directory->d_name, ".") != 0)

unsigned long long fs_list_count(unsigned long long _path) {
	GET_PATH((char*) _path);
	unsigned long long count = 0;

	DIR* dp = opendir(path);
	struct dirent* directory;

	if (dp) {
		while ((directory = readdir(dp)) != NULL) {
			count += FS_LIST_D_NAME_VALID;

		}

		closedir(dp);
		return count;

	} else {
		ALOGW("WARNING Directory `%s` could not be opened (for list counting)\n", path);
		return (unsigned long long) -1;

	}

}

#endif //ANDROID_FS_LIST_H
