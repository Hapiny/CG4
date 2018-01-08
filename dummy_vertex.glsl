#version 330 core
layout (location = 0) in vec3 vertex;

out vec3 texcoords;

uniform vec3 vPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertex + vPos, 1.0f);
	texcoords = normalize(vertex.xyz);
}