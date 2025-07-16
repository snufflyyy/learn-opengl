#version 330 core

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragment_color;

uniform vec3 view_position;
uniform Material material;

uniform DirectionalLight directional_light;

vec3 calculate_directional_light(DirectionalLight directional_light, vec3 normal, vec3 view_direction);

void main() {
    vec3 normalized_normal = normalize(normal);
    vec3 view_direction = normalize(view_position - fragment_position);

    vec3 resulting_color = calculate_directional_light(directional_light, normalized_normal, view_direction);
    fragment_color = vec4(resulting_color, 1.0);
}

vec3 calculate_directional_light(DirectionalLight directional_light, vec3 normal, vec3 view_direction) {
    vec3 light_direction = normalize(-directional_light.direction);
    float diffuse = max(dot(normal, light_direction), 0.0);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    vec3 ambient_light = directional_light.ambient * vec3(texture(material.diffuse, texture_coord));
    vec3 diffuse_light = directional_light.diffuse * diffuse * vec3(texture(material.diffuse, texture_coord));
    vec3 specular_light = directional_light.specular * specular * vec3(texture(material.specular, texture_coord));

    return (ambient_light + diffuse_light + specular_light);
}
