#version 330 core

out vec4 fragment_color;

in vec2 texture_coord;

uniform sampler2D input_texture;

void main() {
    fragment_color = texture(input_texture, texture_coord);
}
