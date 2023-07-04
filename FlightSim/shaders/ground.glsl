#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_NormalTransform;
uniform mat4 u_VP; 

out vec4 v2f_Pos;
out vec3 v2f_Normal;
out vec2 v2f_Texture;

void main()
{
	v2f_Pos = u_ModelMatrix * position;
	v2f_Normal = mat3(u_NormalTransform) * normal;
	gl_Position = u_VP * v2f_Pos;
	v2f_Texture = vec2(textureCoords.x*25, textureCoords.y*25);
	
};

#shader fragment

#version 460 core

uniform vec3 u_Color;
uniform vec3 u_CameraPosition;

in vec4 v2f_Pos;
in vec3 v2f_Normal;
in vec2 v2f_Texture;

out vec4 color;

uniform sampler2D texture1;

void main()
{
	color = texture(texture1, v2f_Texture);
};

