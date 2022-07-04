#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform vec2 texScale;

out vec2 vTexCoord;

void main() 
{
    gl_Position = transform * vec4(aPos, 1.0);

    vTexCoord = aTexCoord * texScale;
}
