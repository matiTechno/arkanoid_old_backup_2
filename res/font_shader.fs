#version 330

in vec2 texCoord;

out vec4 color;

uniform vec4 spriteColor;

uniform sampler2D sampl;

void main()
{
    color = vec4(1, 1, 1, texture(sampl, texCoord).r) * spriteColor;
}
