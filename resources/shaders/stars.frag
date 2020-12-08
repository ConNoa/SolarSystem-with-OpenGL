#version 150

in  vec3 pass_Color;
out vec4 out_Colorx;

void main() {
  out_Colorx = vec4(pass_Color, 1.0);
}
