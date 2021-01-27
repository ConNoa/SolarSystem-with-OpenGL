// #version 150
//
// precision mediump float;
//
// smooth in  vec3 pass_TexCoord;
//
// uniform samplerCube ColorTex;
//
// out vec4 out_Color;
//
//
// void main() {
//   out_Color = texture(ColorTex, vec3(pass_TexCoord));
// }


#version 330
uniform samplerCube uTexture;

smooth in vec3 eyeDirection;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(uTexture, eyeDirection);
}
