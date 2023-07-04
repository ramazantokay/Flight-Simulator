#shader vertex
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 texCoord;

void main()
{
    texCoord = position;
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(position,1.0);
    gl_Position = pos.xyww;
};

#shader fragment
#version 460 core

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, texCoord);
};