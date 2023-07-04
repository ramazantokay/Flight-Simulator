#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_NormalTransform;
uniform mat4 u_VP; 

out vec4 v2f_Pos;
out vec3 v2f_Normal;

void main()
{
	v2f_Pos = u_ModelMatrix * position;
	gl_Position = u_VP * v2f_Pos;

	v2f_Normal = mat3(u_NormalTransform) * normal;
	
};

#shader fragment

#version 460 core

uniform vec3 u_Color;
uniform vec3 u_CameraPosition;

in vec4 v2f_Pos;
in vec3 v2f_Normal;

out vec4 color;

vec3 lightPos = vec3(-10.0f, 60.0f, 250.0f);
vec3 specularColor = vec3(0.5,0.5, 0.5);

void main()
{
	vec3 toLight = normalize(lightPos - v2f_Pos.xyz); 
	vec3 toCam = normalize(u_CameraPosition - v2f_Pos.xyz);
	vec3 halfVector = normalize(toLight+toCam);
	vec3 normal = normalize(v2f_Normal);

	vec3 diffuse = max(0.0, dot(normal, toLight)) * u_Color;
	vec3 specular = pow( max(0,dot(normal, halfVector)) , 50) * specularColor ;

	color = vec4(diffuse + specular + vec3(0.2), 1.0);
};

