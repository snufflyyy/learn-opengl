#version 330 core

out vec4 fragment_color;

// hard coded just as an example
float near = 0.01;
float far = 1000.0;

float linearize_depth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth = linearize_depth(gl_FragCoord.z) / far;
    fragment_color = vec4(vec3(depth), 1.0);
}
