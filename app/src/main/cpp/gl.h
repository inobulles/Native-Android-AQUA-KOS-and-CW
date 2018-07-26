//
// Created by obiwac on 15.07.18.
//

#ifndef ANDROID_GL_H
#define ANDROID_GL_H

bool check_gl_error(const char* function_name) {
	GLint error = glGetError();

	if (error != GL_NO_ERROR) {
		ALOGE("WARNING GL error after `%s()` (error = 0x%08x)\n", function_name, error);
		return true;

	}

	return false;
}

GLuint create_shader(GLenum shader_type, const char* code) {
	GLuint shader = glCreateShader(shader_type);

	if (!shader) {
		check_gl_error("glCreateShader");
		return 0;

	}

	glShaderSource(shader, 1, &code, NULL);
	GLint compiled = GL_FALSE;

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 0) {
			GLchar* info_log = (GLchar*) malloc((size_t) info_log_length);

			if (info_log) {
				glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
				ALOGE("ERROR Could not compile %s shader\n%s\n", shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment", info_log);
				free(info_log);

			}

		}

		glDeleteShader(shader);
		return 0;

	}

	return shader;
}

GLuint create_program(const char* vertex_code, const char* fragment_code) {
	GLuint vertex_shader = 0;
	GLuint fragment_shader = 0;

	GLuint program = 0;
	GLint linked = GL_FALSE;

	vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_code);

	if (vertex_shader) {
		fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_code);

		if (fragment_shader) {
			program = glCreateProgram();

			if (!program) {
				check_gl_error("glCreateProgram");

			} else {
				glAttachShader(program, vertex_shader);
				glAttachShader(program, fragment_shader);

				glLinkProgram(program);
				glGetProgramiv(program, GL_LINK_STATUS, &linked);

				if (!linked) {
					ALOGE("ERROR Could not link program");

					GLint info_log_length = 0;
					glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

					if (info_log_length) {
						GLchar* info_log = (GLchar*) malloc((size_t) info_log_length);

						if (info_log) {
							glGetProgramInfoLog(program, info_log_length, NULL, info_log);
							ALOGE("Could not link program:\n%s\n", info_log);
							free(info_log);

						}

					}

					glDeleteProgram(program);
					program = 0;

				}

			}

		}

	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

static void print_gl_string(const char* name, GLenum s) {
	const char* v = (const char*)glGetString(s);
	ALOGV("GL %s: %s\n", name, v);
}

#endif
