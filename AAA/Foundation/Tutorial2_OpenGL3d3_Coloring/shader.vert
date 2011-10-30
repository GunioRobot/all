#version 330
 
in vec3 vColor;
in vec3 vVertex;
smooth out vec4 vSmoothColor;

uniform mat4 MVP;
void main()
{
   vSmoothColor = vec4(vColor,1);
   gl_Position = MVP*vec4(vVertex,1);
}