#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

uniform sampler2D ourTexture0;//grass
uniform sampler2D ourTexture1;//sand
uniform sampler2D ourTexture2;//mountain

uniform int mode;
uniform int fog;
uniform vec2 heights;

void main()
{

	float h = (heights.y - heights.x) / 10.0f;
	float value = 0.0f;
	vec3 lightDir = vec3(1.0f, 1.0f, 0.0f); 

	vec4 col = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	if (mode == 1)
	{
		if (vFragPosition.y < h + 0.2f)
		{
			col = texture(ourTexture1, vTexCoords * 40);
		}
		else if ((vFragPosition.y > h + 0.9f) && (vFragPosition.y < h + 1.5f))
		{	 
			col = texture(ourTexture0, vTexCoords * 40);
		}
		else if ((vFragPosition.y >= h+0.2f) && (vFragPosition.y <= h+0.9f))
		{
			col = mix(texture(ourTexture1, vTexCoords * 40), texture(ourTexture0, vTexCoords * 40), clamp(vFragPosition.y - h, 0.f, 1.f));
		}
		else if ((vFragPosition.y >= h + 1.5f) && (vFragPosition.y <= h + 2.2f))
		{
			col = mix(texture(ourTexture0, vTexCoords * 80), texture(ourTexture2, vTexCoords * 10), clamp(vFragPosition.y - (h + 1.3f), 0.f, 1.f));
		}
		else if (vFragPosition.y > h + 2.2f)
		{
			col = texture(ourTexture2, vTexCoords * 10);
		}
	}
	else
	{
		col = vec4(vNormal.xyz, 1.0f);
	}
	
	float kd = max(dot(vNormal, lightDir), 0.0);
	col.a = 1.0f;
	color = vec4(kd * col);
	
	if (fog == 1)
	{	
		color = mix(color, vec4(0.7f, 0.7f, 0.7f, 1.0f), exp(-pow(7.0 / (gl_FragCoord.z/gl_FragCoord.w), 2.0)));
	}
}