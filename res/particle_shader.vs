#version 330

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 color;
//layout (location = 2) in mat4 model;
layout (location = 2) in vec3 model;

out vec2 texCoord;
out vec4 spriteColor;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy * model.z + model.xy, 0, 1);
    texCoord = vertex.zw;
    spriteColor = color;
}
