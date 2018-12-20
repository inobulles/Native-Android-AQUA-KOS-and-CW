
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_DUMMY_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_DUMMY_H
	
	#include "../macros_and_inclusions.h"
	
	rectangle_t new_rectangle(unsigned long long x, unsigned long long y, unsigned long long width, unsigned long long height) {
		KOS_DEPRECATED
		return *((rectangle_t*) 0);
		
	}
	
	void draw_rectangle(rectangle_t* this) {
		KOS_DEPRECATED
		
	}
	
#endif
