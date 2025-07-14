#version 330 core

in vec3 fragment_position;
in vec3 normal;

out vec4 fragment_color;

uniform vec3 object_color;
uniform vec3 light_color;

uniform vec3 light_position;
uniform vec3 view_position;

void main() {
    // ambient light
    float ambient_light_strength = 0.1;
    vec3 ambient_light = ambient_light_strength * light_color;

    // diffuse light
    vec3 normalized_normal = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);

    float diffuse = max(dot(normalized_normal, light_direction), 0.0);
    vec3 diffuse_light = diffuse * light_color;

    // specular light
    float specular_strength = 0.5;
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normalized_normal);

    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular_light = specular_strength * specular * light_color;

    vec3 resulting_color = (ambient_light + diffuse_light + specular_light) * object_color;
    fragment_color = vec4(resulting_color, 1.0);
}
