
#ifndef GLES3JNI_H
#define GLES3JNI_H 1

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "kos/lib/structs.h"

#if DYNAMIC_ES3
#include "gl3stub.h"
#else

#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif
#endif

#define VERBOSE_OUTPUT 1

#define LOG_TAG "AQUA"

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#if VERBOSE_OUTPUT
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define printf ALOGV

#define MAX_PATH_LENGTH 4096
#define DEFAULT_ASSETS 1

const char* internal_storage_path;
bool is_internal_storage_path_set = false;

static bool load_asset_bytes(const char* path, char** buffer, unsigned long long* bytes) {
	unsigned long long length = strlen(path);

	if (length > MAX_PATH_LENGTH) {
		ALOGV("WARNING Path length (%lld) is longer than MAX_PATH_LENGTH (%d)\n", length, MAX_PATH_LENGTH);
		return true;

	}

	if (DEFAULT_ASSETS) {
		extern AAssetManager *asset_manager;
		AAsset *asset = AAssetManager_open(asset_manager, path, AASSET_MODE_UNKNOWN);

		if (asset) {
			assert(asset);

			*bytes = (size_t) AAsset_getLength(asset);
			*buffer = (char *) malloc(*bytes + 1);

			AAsset_read(asset, *buffer, *bytes);
			AAsset_close(asset);

			(*buffer)[*bytes] = '\0';
			return false;

		} else {
			ALOGV("WARNING Could not load the ROM from assets/root/ROM. Trying from external storage ...\n");

		}

	}

	if (0) {
		return false;

	} else {
		*bytes = 0;
		*buffer = NULL;

		return true; // problem

	}

}

static bool load_asset(const char* path, char** buffer) {
	unsigned long long bytes;
	return load_asset_bytes(path, buffer, &bytes);

}

extern bool check_gl_error(const char* function_name);
extern GLuint create_shader(GLenum shader_type, const char* code);
extern GLuint create_program(const char* vertex_code, const char* fragment_code);

class Renderer {
    public:
        virtual ~Renderer(void);

        void resize(int w, int h);
        unsigned long long render(void);

		virtual void draw_surface(surface_t* __this) = 0;

    protected:
        Renderer(void);

    private:
        unsigned long long step(void);
        unsigned long long last_frame_ns;

};

extern Renderer* create_es2_renderer();
extern Renderer* create_es3_renderer();

#endif