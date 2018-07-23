
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "native-lib.h"
#include "renderer.h"
#include "gl.h"
#include "kos/decoders/bmp.h"

Renderer* renderer = NULL;
AAssetManager* asset_manager = NULL;

const char* internal_storage_path;
bool is_internal_storage_path_set = false;
bool default_assets = false;

extern "C" {
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_init(JNIEnv* env, jobject obj, jobject java_asset_manager);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_resize(JNIEnv* env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_step(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event(JNIEnv* env, jobject obj, jint pointer_index, jint pointer_type, jint x, jint y, jint quit, jint release);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_give_1internal_1storage_1path(JNIEnv* env, jobject obj, jstring path);

};

#ifndef DYNAMIC_ES3
static GLboolean gl3_stub_init(void) {
	return GL_TRUE;

}
#endif

#include "asm/asm.h"
static program_t de_program;

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_init(JNIEnv* env, jobject obj, jobject java_asset_manager) {
	jclass lib = env->FindClass("com/inobulles/obiwac/aqua/Lib");
	jmethodID create_font = env->GetMethodID(lib, "create_font", "");
	jobject result = env->CallObjectMethod(obj, create_font);

	// asset manager stuff

	asset_manager = AAssetManager_fromJava(env, java_asset_manager);
	assert(NULL != asset_manager);

	// gl stuff

	if (renderer) {
		delete renderer;
		renderer = NULL;

	}

	printGlString("Version", GL_VERSION);
	printGlString("Vendor", GL_VENDOR);
	printGlString("Renderer", GL_RENDERER);
	printGlString("Extensions", GL_EXTENSIONS);

	const char* version_string = (const char*) glGetString(GL_VERSION);

	if (strstr(version_string, "OpenGL ES 3.") && gl3_stub_init()) {
		renderer = create_es3_renderer();

	} else if (strstr(version_string, "OpenGL ES 2.")) {
		renderer = create_es2_renderer();

	} else {
		ALOGE("ERROR Unsupported OpenGL ES version (%s)\n", version_string);

	}

	// program stuff

	ALOGV("\nControl passed to the CW\n");
	ALOGV("Entering DE ...\n");

	char* code_buffer;
	unsigned long long buffer_bytes;

	if (load_asset_bytes("root/ROM", &code_buffer, &buffer_bytes)) {
		if (!default_assets) {
			ALOGW("WARNING Could not load the ROM from internal storage. Trying from assets ...\n");
			default_assets = true;

			if (load_asset_bytes("root/ROM", &code_buffer, &buffer_bytes)) {
				ALOGE("ERROR Could not load the ROM from assets either\n");

			}

		} else {
			ALOGE("ERROR Could not load the ROM\n");

		}

	}

	char* original_code_buffer = code_buffer;
	sscanf(code_buffer, "%llu", &de_program.code_length);
	program_alloc(&de_program);

	int digits;

	unsigned long long i;
	for (i = 0; i < strlen(original_code_buffer); i++) {
		if (sscanf(code_buffer, "%llu, %n", &de_program.code[i], &digits) != 1) {
			break;

		}

		code_buffer += digits;

	}

	if (*code_buffer) {
		ALOGW("WARNING ROM reading did not end cleanly (%d)\n", *code_buffer);

	}

	free(original_code_buffer);
	ALOGV("Starting run setup phase ...\n");
	program_run_setup_phase(&de_program);

}

static int loop(void) {
	if (waiting_video_flip) {
		return 0;

	}

	if (program_run_loop_phase(&de_program) && de_program.error_code) {
		ALOGV("DE return code is %d\n", de_program.error_code);
		program_free(&de_program);

		return de_program.error_code;

	} else {
		return 0;

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
	gl_resize = 1;

	gl_width = (unsigned long long) width;
	gl_height = (unsigned long long) height;

	if (renderer) {
		renderer->resize(width, height);

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_step(JNIEnv* env, jobject obj) {
	while (!waiting_video_flip) {
		loop();

	}

	if (renderer) {
		waiting_video_flip = 0;
		gl_fps = renderer->render();

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event(JNIEnv* env, jobject obj, jint pointer_index, jint pointer_type, jint x, jint y, jint quit, jint release) {
	has_the_event_been_updated_in_the_previous_call_to_Java_com_inobulles_obiwac_aqua_Lib_event_question_mark = true;
	event_last_release = release;

	event_quit = quit;
	event_pointer_click_type = pointer_type;

	event_pointer_x = x;
	event_pointer_y = y;

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_give_1internal_1storage_1path(JNIEnv* env, jobject obj, jstring path) {
	internal_storage_path = env->GetStringUTFChars(path, 0);
	is_internal_storage_path_set = true;

	ALOGV("INFO Internal storage path set to `%s`\n", internal_storage_path);

}