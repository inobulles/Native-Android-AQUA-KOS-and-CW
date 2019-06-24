
cmake_minimum_required(VERSION 3.4.1)
# set targetPlatform, will be passed in from gradle when this sample is completed
# openGL Supportability
# platform         status
#   (0 12)           ES2/ES3 not supported
#   [12, 18)         ES2 only; for ES3, app do dynamic load/detection
#                    this applies to the situations that:
#                        - minimum API is set to less than 18. In this case
#                          there is no ES3 header/lib support inside NDK
#                        - the built APK might be running on newer API phones
#                    with dynamic loading of ES3, the same APK would still be able
#                    to use ES3. Otherwise, app would stuck with ES2 even phone is
#                    is newer than the minimum API level (for example, Android-27 etc).
#
#   [18, 24)         ES2 & ES3
#                    If app is built to only support API-18 or later,
#                    set minimum api level to 18 is good enough, NDK supprts ES3
#                    with the right header and lib files. No need to use ES3 dynamic
#                    detection.
#   [24, infinite)   ES2 & ES3 & Vulkan

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-variable -Wno-unused-function -Wno-implicitly-unsigned-literal -D__HAS_CURL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall -Wno-unused-variable -Wno-unused-function -Wno-implicitly-unsigned-literal -D__HAS_CURL")

if (${ANDROID_PLATFORM_LEVEL} LESS 12)
	message(FATAL_ERROR "ERROR OpenGL 2 is not supported before API level 11 (currently using ${ANDROID_PLATFORM_LEVEL}).")
	return()
	
elseif (${ANDROID_PLATFORM_LEVEL} LESS 128) # LESS 18
	add_definitions("-DDYNAMIC_ES3")
	
	set(GL3_STUB_SRC kos/src/platforms/android/gl3_stub.c)
	set(OPENGL_LIB GLESv2)
	
else ()
	set(OPENGL_LIB GLESv3)

endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

#set_target_properties(entry PROPERTIES LINKER_LANGUAGE CXX)

add_library(entry SHARED
            ${GL3_STUB_SRC}
            entry.cpp)

# include inclusion directories needed for entry lib
target_include_directories(entry PUBLIC
            prebuild/include)

# include libraries needed for entry lib
target_link_libraries(entry
            z
            ${CMAKE_CURRENT_SOURCE_DIR}/prebuild/${CMAKE_ANDROID_ARCH_ABI}/libcurl.a
            ${OPENGL_LIB}
            android
            EGL
            log
            m)
