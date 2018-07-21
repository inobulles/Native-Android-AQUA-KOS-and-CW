#version 300 es

precision mediump float;

in vec2 vertex_texture_coord;
in vec4 vertex_colour;

layout(location = 0) out vec4 out_colour;

uniform sampler2D sampler_texture;
uniform int has_texture;

void main() {
	if (has_texture != 0) out_colour = vertex_colour * texture(sampler_texture, vertex_texture_coord);
	else out_colour = vertex_colour;

}
