#version 330 core

out vec4 Fragcolor;

in vec2 texcoord;

uniform vec3 color;

uniform int tex;

uniform sampler2D texsampler;

void main()
{
	if(tex!=1)
		Fragcolor=vec4(color,1.0f);
	else
		Fragcolor=texture(texsampler,texcoord);
}