
#ifndef NATIVE_LIB_H
#define NATIVE_LIB_H

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "kos/lib/structs.h"

#if DYNAMIC_ES3
#include "gl3_stub.h"
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
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#if VERBOSE_OUTPUT
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define printf ALOGV

#define CALLBACK_NO_PARAMS "()V"

#define MAX_PATH_LENGTH 4096

#define GET_PATH(_path) \
	char path[MAX_PATH_LENGTH] = "root/"; \
    strncat(path, (char*) (_path), MAX_PATH_LENGTH - strlen(path)); \

static JNIEnv* callback_env;
static jclass callback_class;
static jobject callback_lib;

typedef struct {
	bool found;
	jmethodID method;

} callback_method_t;

static callback_method_t java_init_lib;

static callback_method_t java_new_font;
static callback_method_t java_font_remove;

static callback_method_t java_get_font_width;
static callback_method_t java_get_font_height;

static callback_method_t java_create_texture_from_font;

static callback_method_t java_read_external_slash_internal_storage_path;
static callback_method_t java_read_external_slash_internal_storage_path_bytes;

static void init_callback_function(callback_method_t* __this, const char* name, const char* params) {
	__this->found = false;
	__this->method = callback_env->GetStaticMethodID(callback_class, name, params);

	if (__this->method == 0) {
		ALOGW("WARNING `%s` method could not be found\n", name);

	} else {
		__this->found = true;

	}

}

#define CALLBACK(address, call_type, ...) ((call_type)(callback_class, (&address)->method, __VA_ARGS__))
#define CALLBACK_VOID(address, ...) (callback_env->CallStaticVoidMethod(callback_class, (&address)->method, __VA_ARGS__))
#define CALLBACK_VOID_NO_PARAMS(address) (callback_env->CallStaticVoidMethod(callback_class, (&address)->method))
#define CALLBACK_INT(address, ...) (callback_env->CallStaticIntMethod(callback_class, (&address)->method, __VA_ARGS__))

static bool load_asset_bytes(const char* path, char** buffer, unsigned long long* bytes) {
	unsigned long long length = strlen(path);

	if (length > MAX_PATH_LENGTH) {
		ALOGV("WARNING Path length (%lld) is longer than MAX_PATH_LENGTH (%d)\n", length, MAX_PATH_LENGTH);
		return true;

	}

	extern bool default_assets;

	if (default_assets) {
		extern AAssetManager* asset_manager;
		AAsset* asset = AAssetManager_open(asset_manager, path, AASSET_MODE_UNKNOWN);

		if (asset) {
			assert(asset);

			*bytes = (size_t) AAsset_getLength(asset);
			*buffer = (char*) malloc(*bytes + 1);

			AAsset_read(asset, *buffer, *bytes);
			AAsset_close(asset);

			(*buffer)[*bytes] = '\0';
			return false;

		} else {
			ALOGW("WARNING Could not load the file from assets/%s. Trying from internal / external storage ...\n", path);

		}

	}

	extern const char* internal_storage_path;
	extern bool is_internal_storage_path_set;

	jlong temp_bytes = CALLBACK(java_read_external_slash_internal_storage_path_bytes, callback_env->CallStaticLongMethod, callback_env->NewStringUTF(path));

	if (temp_bytes >= 0) {
		*bytes = (unsigned long long) temp_bytes;
		*buffer = (char*) callback_env->GetStringUTFChars((jstring) CALLBACK(java_read_external_slash_internal_storage_path, callback_env->CallStaticObjectMethod, callback_env->NewStringUTF(path)), 0);

		return false;

	} else {
		if (!is_internal_storage_path_set) {
			ALOGW("WARNING Internal / external storage path could not be set\n");

		}

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
