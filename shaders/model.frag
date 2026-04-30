#version 450 core

in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D sampler;

void main() {
  frag_color = texture(sampler, tex_coords);
}
