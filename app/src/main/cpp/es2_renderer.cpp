
#include "native-lib.h"
#include <EGL/egl.h>

static char* VERTEX_SHADER;
static char* FRAGMENT_SHADER;

class es2_renderer : public Renderer {
	public:
		es2_renderer(void);
		virtual ~es2_renderer(void);

		bool init(void);
		virtual void draw_surface(surface_t* __this);
		
	private:
		const EGLContext egl_context;
		GLuint shader_program;

		GLint sampler_location;
		GLint has_texture_location;

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
	extern bool default_assets;

	int error = 0;
	default_assets = true;

	error += load_asset("es2_vertex_shader.glsl",   &VERTEX_SHADER);
	error += load_asset("es2_fragment_shader.glsl", &FRAGMENT_SHADER);

	if (error > 0) {
		ALOGE("WARNING Failed to load shader assets, returning error from `es2_renderer::init` ...\n");
		return false;

	}

	default_assets = false;
	shader_program = create_program(VERTEX_SHADER, FRAGMENT_SHADER);

	if (!shader_program) {
		ALOGE("WARNING Failed to create shader program, returning error from `es2_renderer::init` ...\n");
		return false;

	}

	sampler_location     = glGetUniformLocation(shader_program, "sampler_texture");
	has_texture_location = glGetUniformLocation(shader_program, "has_texture");

	glUseProgram(shader_program);
	return true;
	
}

es2_renderer::~es2_renderer(void) {
	if (eglGetCurrentContext() != egl_context) {
		return;
		
	}

	glDeleteProgram(shader_program);

	free(VERTEX_SHADER);
	free(FRAGMENT_SHADER);
	
}

static inline void vertex_attribute_pointer(GLuint index, int vector_size, void* pointer) {
	glVertexAttribPointer(index, vector_size, GL_FLOAT, GL_FALSE, vector_size * sizeof(GLfloat), pointer);

}

extern GLfloat projection_matrix[16];

void es2_renderer::draw_surface(surface_t* __this) {
	glUniform1i(has_texture_location, (GLint) __this->has_texture);

	vertex_attribute_pointer(0, 3, __this->vertices);
	vertex_attribute_pointer(1, 2, __this->texture_coords);
	vertex_attribute_pointer(2, 4, __this->colours);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	if (__this->has_texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(  GL_TEXTURE_2D, (GLuint) __this->texture);
		glUniform1i(sampler_location, 0);

	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, __this->faces);

}