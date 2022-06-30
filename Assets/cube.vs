#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//layout (location = 1) in vec2 aTexCoord;

uniform vec4 color;
uniform mat4 transform;

out vec4 vColor;
//out vec2 vTexCoord;

float map(float inValue, float inMin, float inMax, float outMin, float outMax) 
{
    return outMin + (sign(inValue) - inMin) * (outMax - outMin) / (inMax - inMin);
}

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);

    vColor = vec4(color.xyz * map(aPos.y, -1.0, 1.0, 0.25, 0.92), 1.0);
    if (color.a < 1.0)
        vColor.a = color.a;

    /*vTexCoord = aTexCoord;*/
}
