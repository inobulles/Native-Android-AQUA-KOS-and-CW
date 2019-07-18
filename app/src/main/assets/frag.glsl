#version 300 es

precision mediump float;

in vec2 vertex_texture_coord;
in vec4 vertex_colour;

layout(location = 0) out vec4 out_colour;

uniform sampler2D sampler_texture;
uniform int       has_texture;

void main() {
	vec4 colour;

	if (has_texture != 0) colour = vertex_colour * texture(sampler_texture, vertex_texture_coord);
	else                  colour = vertex_colour;

	if (colour.w > 0.0f) out_colour = colour;
	else                 discard;

}
