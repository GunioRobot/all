#version 330
smooth in vec2 vTexCoord;
out vec4 vFragColor;

uniform sampler2D textureMap;

void main(void)
{
   vFragColor = texture(textureMap, vTexCoord);
}