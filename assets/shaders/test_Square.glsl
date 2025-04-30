#type vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = a_TexCoord;
	gl_Position = projection * view * model * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 a_Color;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	a_Color = texture(ourTexture, TexCoord);
}
