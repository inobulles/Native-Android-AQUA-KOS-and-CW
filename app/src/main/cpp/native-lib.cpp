
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
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_init(                         JNIEnv* env, jobject obj, jobject java_asset_manager);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_dispose_1all(                 JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_resize(                       JNIEnv* env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_step(                         JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event(                        JNIEnv* env, jobject obj, jint pointer_index, jint pointer_type, jint x, jint y, jint quit, jint release);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_give_1internal_1storage_1path(JNIEnv* env, jobject obj, jstring path);

};

#ifndef DYNAMIC_ES3
static GLboolean gl3_stub_init(void) {
	return GL_TRUE;

}
#endif

void mfree(void* pointer, unsigned long long bytes) {
	free(pointer);

}

#include "alog.h"
#include "asm/asm.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

static program_t*         current_de_program;
static char*              rom_data;
static unsigned long long rom_bytes;

signed long long load_rom(const char* path) {
	ALOGE("WARNING ROM loading (%s) is not supported on this platform\n", __func__);
	return -1;

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_init(JNIEnv* env, jobject obj, jobject java_asset_manager) {
	callback_env = env;
	jclass _class = env->FindClass("com/inobulles/obiwac/aqua/Lib");
	callback_class = (jclass) env->NewGlobalRef(_class);
	callback_lib = env->NewGlobalRef(callback_class);

#define CALLBACK_FONT_AND_TEXT "(ILjava/lang/String;)I"

	init_callback_function(&java_init_lib, "init_lib", CALLBACK_NO_PARAMS);
	CALLBACK_VOID_NO_PARAMS(java_init_lib);

	init_callback_function(&java_new_font, "new_font", "(ILjava/lang/String;)I");
	init_callback_function(&java_font_remove, "font_remove", "(I)V");

	init_callback_function(&java_get_font_width, "get_font_width", CALLBACK_FONT_AND_TEXT);
	init_callback_function(&java_get_font_height, "get_font_height", CALLBACK_FONT_AND_TEXT);

	init_callback_function(&java_create_texture_from_font, "create_texture_from_font",
						   "(ILjava/lang/String;)[B");

	init_callback_function(&java_read_external_slash_internal_storage_path,
						   "read_external_slash_internal_storage_path",
						   "(Ljava/lang/String;)Ljava/lang/String;");
	init_callback_function(&java_read_external_slash_internal_storage_path_bytes,
						   "read_external_slash_internal_storage_path_bytes",
						   "(Ljava/lang/String;)J");

	// asset manager stuff

	asset_manager = AAssetManager_fromJava(env, java_asset_manager);
	assert(NULL != asset_manager);

	// gl stuff

	if (renderer) {
		delete renderer;
		renderer = NULL;

	}

	ALOGI("OpenGL info\n");
	ALOGI("\tVendor:                   %s\n", glGetString(GL_VENDOR));
	ALOGI("\tRenderer:                 %s\n", glGetString(GL_RENDERER));
	ALOGI("\tVersion:                  %s\n", glGetString(GL_VERSION));
	ALOGI("\tShading language version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
//	ALOGI("\tExtensions:               %s\n", glGetString(GL_EXTENSIONS));

	const char *version_string = (const char *) glGetString(GL_VERSION);

	if (strstr(version_string, "OpenGL ES 3.") && gl3_stub_init()) {
		renderer = create_es3_renderer();

	} else if (strstr(version_string, "OpenGL ES 2.")) {
		renderer = create_es2_renderer();

	} else {
		ALOGE("ERROR Unsupported OpenGL ES version (%s)\n", version_string);

	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA);

	//~ ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 500.0f);

	//~ glHint(GL_POINT_SMOOTH, GL_NICEST);
	//~ glHint(GL_LINE_SMOOTH, GL_NICEST);
	//~ glHint(GL_POLYGON_SMOOTH, GL_NICEST);

	//~ glEnable(GL_POINT_SMOOTH);
	//~ glEnable(GL_LINE_SMOOTH);
	//~ glEnable(GL_POLYGON_SMOOTH);

	// program stuff

	ALOGI("\nControl passed to the CW\n");
	ALOGI("Entering DE ...\n");

	program_t* de_program = (program_t*) malloc(sizeof(program_t));
	current_de_program = de_program;

	default_assets = true; /// REMME

	if (load_asset_bytes("root/rom.zed", &rom_data, &rom_bytes)) {
		if (!default_assets) {
			ALOGW("WARNING Could not load the ROM from internal / external storage. Trying from assets ...\n");
			default_assets = true;

			if (load_asset_bytes("root/rom.zed", &rom_data, &rom_bytes)) {
				ALOGE("ERROR Could not load the ROM from assets either\n");

			}

		} else {
			ALOGE("ERROR Could not load the ROM\n");

		}

	}

	ALOGI("Setting up predefined_textures ...\n");
	int warning = kos_setup_predefined_textures();

	if (warning) {
		ALOGW("WARNING Problem occurred whilst setting up predefined textures (failed %d textures)\n", warning);

	}

	current_de_program->pointer = rom_data;
	ALOGI("Starting run setup phase ...\n");
	program_run_setup_phase(current_de_program);

}

static int loop(void) {
	if (waiting_video_flip) {
		return 0;

	}

	if (program_run_loop_phase(current_de_program) && current_de_program->error_code) {
		ALOGV("DE return code is %d\n", current_de_program->error_code);
		program_free(current_de_program);
		free(current_de_program->base_pointer/*, rom_bytes*/);

		return current_de_program->error_code;

	} else {
		return 0;

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_dispose_1all(JNIEnv* env, jobject obj) {
	kos_free_predefined_textures();

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

	ALOGV("INFO Internal / external storage path set to `%s`\n", internal_storage_path);

}
