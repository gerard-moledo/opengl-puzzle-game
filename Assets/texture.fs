#version 300 es

out mediump vec4 FragColor;

in mediump vec2 vTexCoord;

uniform sampler2D textureSampler;
uniform mediump vec4 color;

void main()
{
    FragColor = texture(textureSampler, vTexCoord) * color;
}
