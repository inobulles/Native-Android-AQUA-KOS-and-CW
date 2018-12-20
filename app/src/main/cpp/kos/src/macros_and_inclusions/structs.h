
#ifndef __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_STRUCTS_H
	#define __AQUA__SDL2_SRC_MACROS_AND_INCLUSIONS_STRUCTS_H
	
	typedef struct {
		int warning_count;
		
		int width;
		int height;
		
		int bpp;
		
		#if KOS_USES_SDL2
			SDL_Window*   window;
			SDL_GLContext context;
		#endif
		
		#if KOS_USES_OPENGL
			GLuint shader_program;
		#endif
		
		#if KOS_USES_BCM && KOS_USES_OPENGLES
			EGLDisplay display;
			EGLSurface surface;
			EGLContext context;
		#endif
		
	} kos_t;
	
#endif
