#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D water_texture;
uniform int fog;

void main()
{
	color = texture(water_texture, TexCoord * 40);
	//color = vec4(0.22f, 0.57f, 0.87f, 0.7f);
	if (fog == 1)
	color = mix(color, vec4(0.7f, 0.7f, 0.7f, 1.0f), exp(-pow(10.0 / (gl_FragCoord.z/gl_FragCoord.w), 2.0)));
	
	color.a = 0.7f;
}