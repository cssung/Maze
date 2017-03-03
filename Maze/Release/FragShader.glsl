#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
}; 

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragNormal;
in vec3 fragPos;
in vec3 lightPos;
out vec4 color;

uniform Material material;
uniform Light light;  
uniform vec3 cameraPos;

void main() 
{
	vec3 ambientLight = light.ambient * material.ambient;

    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 diffuseLight = light.diffuse * (diffuse * material.diffuse);

	vec3 cameraDir = normalize(-fragPos);
	vec3 refelctionDir = reflect(-lightDir, normal); 
	float specular = pow(max(dot(cameraDir, refelctionDir), 0.0), material.shininess);
    vec3 specularLight = light.specular * (specular * material.specular);  

	vec3 light = (ambientLight + diffuseLight + specularLight);
    color = vec4(light, 1.0f);
}

