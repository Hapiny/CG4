#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform int fog;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
    if (fog == 1)
	{	
		FragColor = mix(FragColor, vec4(0.7f, 0.7f, 0.7f, 1.0f), exp(-pow(7.0 / (gl_FragCoord.z/gl_FragCoord.w), 2.0)));
	}
}

