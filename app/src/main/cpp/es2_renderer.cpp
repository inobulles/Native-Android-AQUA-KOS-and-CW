
#include "native-lib.h"
#include <EGL/egl.h>

class es2_renderer : public Renderer {
	public:
		es2_renderer(void);
		virtual ~es2_renderer(void);

		bool init(void);
		virtual void draw_surface(surface_t* __this);
		
	private:
		const EGLContext egl_context;
		GLuint shader_program;

};

Renderer* create_es2_renderer(void) {
	es2_renderer* renderer = new es2_renderer;
	
	if (!renderer->init()) {
		delete renderer;
		return NULL;
	}
	
	return renderer;
	
}

es2_renderer::es2_renderer(void) : egl_context(eglGetCurrentContext()) {
	shader_program = 0;
	
}

bool es2_renderer::init(void) {
	ALOGV("INFO Using OpenGL ES 2.0 renderer");
	return true;
	
}

es2_renderer::~es2_renderer(void) {
	if (eglGetCurrentContext() != egl_context) {
		return;
		
	}

	glDeleteProgram(shader_program);
	
}

void es2_renderer::draw_surface(surface_t* __this) {
	printf("TODO `es2_renderer::draw_surface` __this = (surface_t*) %p\n", __this);

}