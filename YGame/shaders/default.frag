#version 150 core

in float pz;

out vec4 out_Color;

void main(void) {
      out_Color = vec4(pz / 700, pz / 500, pz / 300, 1.0);
}
