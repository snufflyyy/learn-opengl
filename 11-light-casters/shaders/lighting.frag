#version 330 core

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 fragment_color;

uniform vec3 view_position;
uniform Material material;
uniform Light light;

void main() {
    // ambient light
    vec3 ambient_light = light.ambient * vec3(texture(material.diffuse, texture_coord));

    // diffuse light
    vec3 normalized_normal = normalize(normal);
    vec3 light_direction = normalize(light.position - fragment_position);
    float diffuse = max(dot(normalized_normal, light_direction), 0.0);
    vec3 diffuse_light = light.diffuse * diffuse * vec3(texture(material.diffuse, texture_coord));

    // specular light
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normalized_normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular_light = light.specular * specular * vec3(texture(material.specular, texture_coord));

    // attenuation
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient_light *= attenuation;
    diffuse_light *= attenuation;
    specular_light *= attenuation;

    vec3 resulting_color = ambient_light + diffuse_light + specular_light;
    fragment_color = vec4(resulting_color, 1.0);
}
