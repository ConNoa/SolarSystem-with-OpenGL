#version 150

precision mediump float;

smooth in  vec3 pass_TexCoord;

uniform samplerCube ColorTex;

out vec4 out_Color;


void main() {
  out_Color = texture(ColorTex, vec3(pass_TexCoord));
}
