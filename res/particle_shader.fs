#version 330

in vec2 texCoord;

in vec4 spriteColor;

out vec4 color;

uniform sampler2D sampl;

void main()
{
    color = texture(sampl, texCoord) * spriteColor;
}
