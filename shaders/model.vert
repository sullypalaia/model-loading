#version 450 core

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_coords_in;
layout (location = 2) in vec3 normal_in;

out vec2 tex_coords;

layout (std140, binding = 0) uniform camera_data {
    mat4 projection;
    mat4 view;
};

void main() {
  tex_coords = tex_coords_in;

  gl_Position = projection * view * vec4(pos_in, 1.0);
}
