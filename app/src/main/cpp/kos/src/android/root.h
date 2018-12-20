//
// Created by obiwac on 20/12/18.
//

#ifndef ANDROID_FUNCTIONS_H
#define ANDROID_FUNCTIONS_H

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "native-lib.h"
#include "renderer.h"
#include "gl.h"

Renderer*      renderer      = nullptr;
AAssetManager* asset_manager = nullptr;

const char* internal_storage_path;
bool     is_internal_storage_path_set = false;

bool default_assets = false;

#define PKG_ORG inobulles
#define PKG_PER obiwac
#define PKG_APP aqua

#define   str(x) __str(x)
#define __str(x) #x

#define   glue(x, y) __glue(x, y)
#define __glue(x, y) x##y

#define JNI_FUNCTION_NAME(name) glue(glue(glue(glue(Java_com_, PKG_ORG), glue(_, PKG_PER)), glue(_, PKG_APP)), glue(_Lib_, name))

extern "C" {
	JNIEXPORT void JNICALL JNI_FUNCTION_NAME(give_1text_1input_1response)  (JNIEnv* env, jobject obj, jboolean has_response, jstring, response);
	JNIEXPORT void JNICALL JNI_FUNCTION_NAME(give_1log_1tag)               (JNIEnv* env, jobject obj, jstring tag, jboolean standalone);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(init)                         (JNIEnv* env, jobject obj, jobject java_asset_manager);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(dispose_1all)                 (JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(start)                        (JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(resize)                       (JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(step)                         (JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(event)                        (JNIEnv* env, jobject obj, jint pointer_index, jint pointer_type, jint x, jint y, jint quit, jint release, jint tray_offset);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(event_1macro)                 (JNIEnv* env, jobject obj, jint macro, jint set);
    JNIEXPORT void JNICALL JNI_FUNCTION_NAME(give_1internal_1storage_1path)(JNIEnv* env, jobject obj, jstring path);
}

#ifndef DYNAMIC_ES3
	static GLboolean gl3_stub_init(void) {
		return GL_TRUE;

	}
#endif

void mfree(void* pointer, unsigned long long bytes) {
	free(pointer);

}

#include "alog.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#ifndef     __LOG_TAG
	#define __LOG_TAG "AQUA"
	char*     LOG_TAG = (char*) __LOG_TAG;
#endif

JNIEXPORT void JNICALL JNI_FUNCTION_NAME(give_1log_1tag)(JNIEnv* env, jobject obj, jstring tag, jboolean standalone) {
	default_assets = (bool) standalone;

	jboolean is_copy = 0;
	LOG_TAG = (char*) env->GetStringUTFChars(tag, &is_copy);

}

#include "../../glue.c"

JNIEXPORT void JNICALL JNI_FUNCTION_NAME(init)(JNIEnv* env, jobject obj, jobject java_asset_manager) {
	callback_env   =          env;
	jclass _class  =          env->FindClass("com/" str(PKG_ORG) "/" str(PKG_PER) "/" str(PKG_APP) "/Lib");
	callback_class = (jclass) env->NewGlobalRef(_class);
	callback_lib   =          env->NewGlobalRef(callback_class);

	#define CALLBACK_FONT_AND_TEXT "(ILjava/lang/String;)I"

	init_callback_function(&java_init_lib, "init_lib", CALLBACK_NO_PARAMS);
	CALLBACK_VOID_NO_PARAMS(java_init_lib);

	init_callback_function(&java_new_font,    "new_font",    "(ILjava/lang/String;)I");
	init_callback_function(&java_font_remove, "font_remove", "(I)V");

	init_callback_function(&java_get_font_width,  "get_font_width",  CALLBACK_FONT_AND_TEXT);
	init_callback_function(&java_get_font_height, "get_font_height", CALLBACK_FONT_AND_TEXT);

	init_callback_function(&java_create_texture_from_font, "create_texture_from_font", "(ILjava/lang/String;II)I");

	init_callback_function(&java_read_external_slash_internal_storage_path,       "read_external_slash_internal_storage_path",       "(Ljava/lang/String;)Ljava/lang/String;");
	init_callback_function(&java_read_external_slash_internal_storage_path_bytes, "read_external_slash_internal_storage_path_bytes", "(Ljava/lang/String;)J");

	init_callback_function(&java_package_exists, "package_exists", "(Ljava/lang/String;)I");
	init_callback_function(&java_package_open,   "package_open",   "(Ljava/lang/String;)V");

	init_callback_function(&java_open_text_input, "open_text_input", CALLBACK_NO_PARAMS);

	// asset manager stuff

	asset_manager = AAssetManager_fromJava(env, java_asset_manager);
	assert(asset_manager != NULL);

	main(2, (void*[]) {"./a.out", "assets/root/rom.zed"});

}

#endif //ANDROID_FUNCTIONS_H
