#version 330 core

out vec4 FragColor;

in vec4 vColor;/*
in vec2 vTexCoord; */

/* uniform sampler2D textureSampler; */

void main() 
{
    FragColor = vColor;
}