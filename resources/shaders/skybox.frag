#version 150

smooth in  vec3 pass_cam_dir;
out vec4 out_Color;

uniform samplerCube SkyTexture;

void main() {
  out_Color = texture(SkyTexture, pass_cam_dir);
}
