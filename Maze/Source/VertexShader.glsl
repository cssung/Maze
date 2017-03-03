#version 330 core

in vec4 vPosition;
in vec3 normal;
out vec3 fragNormal;
out vec3 fragPos;
out vec3 lightPos;

uniform vec3 light;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	fragNormal = mat3(transpose(inverse(view * model))) * normal;  
	fragPos = vec3(view * model * vPosition);
	lightPos = vec3(view * vec4(light, 1.0f)); 
	gl_Position = projection * view * model * vPosition;
} 
