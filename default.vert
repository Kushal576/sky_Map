#version 330 core

layout (location=0) in vec3 pos;
layout (location=1) in vec2 texcoordinates;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 textransform;

void main()
{
	gl_Position=projection*view*model*vec4(pos.x,pos.y,pos.z,1.0f);
	vec2 tex;
	tex.x=texcoordinates.x+textransform.x;
	tex.y=texcoordinates.y+textransform.y;
	texcoord=tex;
}
