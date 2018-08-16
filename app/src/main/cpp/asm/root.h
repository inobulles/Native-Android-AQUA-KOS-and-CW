
#ifndef __AQUA__CW_SRC_DECODER_ROOT_H
#define __AQUA__CW_SRC_DECODER_ROOT_H

#define SIGNED_UNSIGNED 0

#if !SIGNED_UNSIGNED
typedef unsigned long long unsigned_t;
#else
typedef signed long long unsigned_t;
#endif

typedef signed long long signed_t;

#if !SIGNED_UNSIGNED
typedef unsigned int unsigned_32bit_t;
typedef unsigned short unsigned_16bit_t;
typedef unsigned char unsigned_8bit_t;
#else
typedef signed int unsigned_32bit_t;
typedef signed short unsigned_16bit_t;
typedef signed char unsigned_8bit_t;
#endif

#endif
