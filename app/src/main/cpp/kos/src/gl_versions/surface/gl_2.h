
#ifndef __AQUA__SDL2_SRC_KOS_GL_VERSIONS_SURFACE_GL_2_H
	#define __AQUA__SDL2_SRC_KOS_GL_VERSIONS_SURFACE_GL_2_H
	
	#include "../../macros_and_inclusions.h"
	#include <time.h>
	
	int glUniform1i();
	int glUniform1f();
	
	int gl2_surface_draw(surface_t* __this) {
		#if KOS_USES_OPENGL_DESKTOP
			if (shader_has_set_locations) {
				glUniform1i(shader_time_location, clock());
				glUniform1i(shader_has_texture_location, (GLint) __this->has_texture);

			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			if (__this->has_texture) {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			}

			glVertexPointer(3, GL_FLOAT, 0, __this->vertices);
			glColorPointer (4, GL_FLOAT, 0, __this->colours);

			if (__this->has_texture) {
				glTexCoordPointer(2, GL_FLOAT, 0, __this->texture_coords);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture  (GL_TEXTURE_2D, (GLuint) __this->texture);

				if (shader_has_set_locations) {
					glUniform1i(shader_sampler_location, 0);

				}

			}

			glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, __this->faces);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

			if (__this->has_texture) {
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			}
		#elif KOS_USES_JNI
			ALOGA("TODO\n");
		#endif
		
		return 0;
		
	}
	
#endif
