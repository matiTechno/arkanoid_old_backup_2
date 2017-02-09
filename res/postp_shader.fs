#version 330

out vec4 color;
in vec2 texCoord;

uniform sampler2D sampl;

void main()
{
    color = vec4(texture(sampl, texCoord).xyz, 1);
}
