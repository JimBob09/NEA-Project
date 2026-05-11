#version 330 core
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
out vec4 FragColor;

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

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uCameraPos;

void main() {
    // Attenuation
    float distance = length(uLight.position - FragPos);
    float attenuation = 1.0 / (uLight.constant + uLight.linear * distance + uLight.quadratic * (distance * distance));

    // Ambient Light
    vec3 ambient = vec3(texture(uMaterial.diffuse, TexCoord)) * uLight.ambient;

    // Diffuse Light
    vec3 normal = normalize(Normal);
    vec3 lightRay = normalize(uLight.position - FragPos);

    float diffuseDiff = max(dot(normal, lightRay), 0.0);
    vec3 diffuse = vec3(texture(uMaterial.diffuse, TexCoord)) * diffuseDiff * uLight.diffuse * attenuation;

    // Specular Lighting
    vec3 viewDirection = normalize(uCameraPos - FragPos);
    vec3 reflectionRay = reflect(-lightRay, normal);  

    float specularDiff = pow(max(dot(viewDirection, reflectionRay), 0.0), uMaterial.shininess);
    vec3 specular =  vec3(texture(uMaterial.specular, TexCoord)) * specularDiff * uLight.specular * attenuation;

    // Phong result
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}