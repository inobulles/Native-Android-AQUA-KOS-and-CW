//
// Created by obiwac on 20.10.18.
//

#ifndef ANDROID_FS_LIST_H
#define ANDROID_FS_LIST_H

#include <dirent.h>
#include "../native-lib.h"

#define FS_LIST_FUNCTIONS 1
#define FS_LIST_D_NAME_VALID (strcmp(directory->d_name, "..") != 0 && strcmp(directory->d_name, ".") != 0)

unsigned long long fs_list_count(unsigned long long _path) { /// TODO new/delete
	return 5;
	/*GET_PATH((char*) _path);
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

	}*/

}

#include <sys/stat.h>

#define FS_LIST_ENTRY_UNKNOWN   0
#define FS_LIST_ENTRY_FILE      1
#define FS_LIST_ENTRY_DIRECTORY 2

char** fs_list(unsigned long long _path) { /// TODO new/delete
	GET_PATH((char*) _path);

	unsigned long long count   = fs_list_count(_path);
	unsigned long long current = 0;
	char**             result  = new char*[count * sizeof(char*)];

	for (int t = 0; t < count; t++) {
		const char* name = "TEST";

		unsigned long long bytes = strlen(name) + 1;
		result[current] = new char[bytes * sizeof(char) + sizeof(unsigned long long)];
		memcpy((result[current] + sizeof(unsigned long long)), name, bytes);
		*((unsigned long long*) result[current]) = FS_LIST_ENTRY_FILE;

	}

	return result;

	/*GET_PATH((char*) _path);

	unsigned long long count   = fs_list_count(_path);
	unsigned long long current = 0;
	char**             result  = (char**) malloc(count * sizeof(char*));

	DIR* dp = opendir(path);
	struct dirent* directory;

	if (dp) {
		while ((directory = readdir(dp)) != NULL) {
			if (FS_LIST_D_NAME_VALID) {
				unsigned long long bytes = strlen(directory->d_name) + 1;
				result[current] = (char*) malloc(bytes * sizeof(char) + sizeof(unsigned long long));
				memcpy((result[current] + sizeof(unsigned long long)), directory->d_name, bytes);

#define RESULT_FILE_TYPE *((unsigned long long*) result[current])
				RESULT_FILE_TYPE = FS_LIST_ENTRY_UNKNOWN;

				if (directory->d_type == DT_UNKNOWN) {
					struct stat path_stat;
					stat(directory->d_name, &path_stat);

					if      (S_ISDIR(path_stat.st_mode)) RESULT_FILE_TYPE = FS_LIST_ENTRY_DIRECTORY;
					else if (S_ISREG(path_stat.st_mode)) RESULT_FILE_TYPE = FS_LIST_ENTRY_FILE;

				}

				else if (directory->d_type == DT_DIR)    RESULT_FILE_TYPE = FS_LIST_ENTRY_DIRECTORY;
				else if (directory->d_type == DT_REG)    RESULT_FILE_TYPE = FS_LIST_ENTRY_FILE;

			}

		}

		closedir(dp);
		return result;

	} else {
		ALOGW("WARNING Directory `%s` could not be opened (for listing)\n", path);
		return (char**) 0;

	}*/

}

void fs_list_free(unsigned long long list, unsigned long long count) {
	unsigned long long i;
	for (i = 0; i < count; i++) {
		delete[] ((char**) list)[i];

	}

	delete[] (char**) list;

}

#endif //ANDROID_FS_LIST_H
