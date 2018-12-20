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

#endif //ANDROID_FUNCTIONS_H
