#version 330 core

in vec3 fragment_position;
in vec3 normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 fragment_color;

uniform vec3 view_position;

void main() {
    // ambient light
    vec3 ambient_light = light.ambient * material.ambient;

    // diffuse light
    vec3 normalized_normal = normalize(normal);
    vec3 light_direction = normalize(light.position - fragment_position);
    float diffuse = max(dot(normalized_normal, light_direction), 0.0);
    vec3 diffuse_light = light.diffuse * (diffuse * material.diffuse);

    // specular light
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normalized_normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular_light = light.specular * (specular * material.specular);

    vec3 resulting_color = ambient_light + diffuse_light + specular_light;
    fragment_color = vec4(resulting_color, 1.0);
}
