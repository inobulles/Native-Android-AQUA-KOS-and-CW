#version 300 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec4 colour;

out vec2 vertex_texture_coord;
out vec4 vertex_colour;

//uniform vec4 projection_matrix;

void main() {
	gl_Position = position;//TODO * projection_matrix;

	vertex_colour = colour;
	vertex_texture_coord = texture_coord;

}
