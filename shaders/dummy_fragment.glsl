#version 330 core

in vec3 texcoords;

out vec4 color;

uniform int fog;
uniform samplerCube cube;

void main()
{
	//color = vec4(0.0f, 0.2f, 0.8f, 1.0f);
	color = texture(cube, texcoords);
	if (fog == 1)
	color = mix(color, vec4(0.7f, 0.7f, 0.7f, 1.0f), exp(-pow(10.0 / (gl_FragCoord.z/gl_FragCoord.w), 2.0)));
}