#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture_coord;

out vec3 fragment_position;
out vec3 normal;
out vec2 texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
    fragment_position = vec3(model * vec4(a_position, 1.0));
    mat3 normal_matrix = transpose(inverse(mat3(model)));
    normal = normalize(normal_matrix * a_normal);
    texture_coord = a_texture_coord;
}
