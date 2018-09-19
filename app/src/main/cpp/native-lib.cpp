
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
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_start(                        JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_resize(                       JNIEnv* env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_step(                         JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event(                        JNIEnv* env, jobject obj, jint pointer_index, jint pointer_type, jint x, jint y, jint quit, jint release, jint tray_offset);
	JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event_1macro(                 JNIEnv* env, jclass type, jint macro, jint set);
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
#include "asm/thread.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

typedef struct {
	program_t* program;

	char*              rom_data;
	unsigned long long rom_bytes;

} de_t;

#define MAX_DE_COUNT 16
static signed long long de_index;

static de_t* current_de;
static de_t  des[MAX_DE_COUNT];

signed long long load_rom(unsigned long long path) {
	current_de = &des[++de_index];
	current_de->rom_data = (char*) 0;

	if (load_asset_bytes((const char*) path, &current_de->rom_data, &current_de->rom_bytes)) {
		if (!default_assets) {
			ALOGW("WARNING Could not load the ROM from internal / external storage. Trying from assets ...\n");
			default_assets = true;

			if (load_asset_bytes((const char*) path, &current_de->rom_data, &current_de->rom_bytes)) {
				ALOGE("ERROR Could not load the ROM from assets neither\n");

			}

		} else {
			ALOGE("ERROR Could not load the ROM\n");

		}

	}

	if (current_de->rom_data == (char*) 0) {
		de_index--;
		return -1;

	} else {
		current_de->program = (program_t*) malloc(sizeof(program_t));
		ALOGI("Starting run setup phase ...\n");

		program_run_setup_phase(current_de->program);
		return 0;

	}

}

bool rom_free_last(void) {
	program_free(current_de->program);

	free(current_de->program);
	free(current_de->rom_data);

	current_de = &des[--de_index];
	current_de->program->main_thread.registers[REGISTER_FAMILY_a] = (reg_t) current_de->program->error_code;

	return de_index < 0;

}

bool disable_gl = false;
bool already_disposed = true;

void nothing(...) {

}

void rom_init(JNIEnv* env, jobject obj) {
	already_disposed = false;

	// gl stuff

	disable_gl = false;
	event_quit = false;

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

	const char* version_string = (const char*) glGetString(GL_VERSION);

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

#define DEFAULT_ROM_PATH "root/rom.zed"
	load_rom((unsigned long long) DEFAULT_ROM_PATH);

	ALOGI("Setting up predefined_textures ...\n");
	int warning = kos_setup_predefined_textures();

	if (warning) {
		ALOGW("WARNING Problem occurred whilst setting up predefined textures (failed %d textures)\n", warning);

	}

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

	de_index = -1;

	program_run_global_init_phase();
	rom_init(env, obj);

}

static int loop(void) {
	if (waiting_video_flip) {
		return -1;

	}

	if (program_run_loop_phase(current_de->program)) {
		ALOGV("DE return code is %d\n", current_de->program->error_code);
		return rom_free_last() ? current_de->program->error_code : -1;

	} else {
		return -1;

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_start(JNIEnv* env, jobject obj) {
	rom_init(env, obj);

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_dispose_1all(JNIEnv* env, jobject obj) {
	if (already_disposed) {
		return;

	}

	already_disposed = true;

	ALOGI("Forcing program to exit ...\n");
	disable_gl = true;

	event_quit = 1;
	int return_value;

	while (1) {
		return_value = loop();
		waiting_video_flip = 0;

		if (return_value >= 0) {
			break;

		}

	}

	ALOGI("Freeing other things (return_value = %d) ...\n", return_value);
	kos_free_predefined_textures();

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
	gl_resize = 1;

	gl_width  = (unsigned long long) width;
	gl_height = (unsigned long long) height;

	if (renderer) {
		renderer->resize(width, height);

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_step(JNIEnv* env, jobject obj) {
	while (!waiting_video_flip) {
		int return_value = loop();

		if (return_value >= 0) {
			exit(return_value);

		}

	}

	if (renderer) {
		waiting_video_flip = 0;
		gl_fps = renderer->render();

	}

}

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event(JNIEnv* env, jobject obj, jint pointer_index, jint pointer_type, jint x, jint y, jint quit, jint release, jint tray_offset) {
	extern int notification_tray_offset;
	notification_tray_offset = (int) tray_offset;

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

JNIEXPORT void JNICALL Java_com_inobulles_obiwac_aqua_Lib_event_1macro(JNIEnv* env, jclass type, jint macro, jint set) {
	has_the_event_been_updated_in_the_previous_call_to_Java_com_inobulles_obiwac_aqua_Lib_event_question_mark = true;

	event_pointer_click_type = set;
	event_last_release       = set;

	//event_macros[macro] = set; /// TODO

}