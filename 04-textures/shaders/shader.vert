#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture_coord;

out vec2 texture_coord;

void main() {
    gl_Position = vec4(a_position, 1.0f);
    texture_coord = a_texture_coord;
}
