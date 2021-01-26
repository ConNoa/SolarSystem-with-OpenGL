#version 150

in  vec3 pass_Normal;
in  vec3 pass_vert_pos;
in  vec3 pass_vert_pos_world;
in  vec2 pass_TexCoord;

uniform sampler2D YourTexture;

#define PI 3.1415
out vec4 out_Color;
void main() {

  //we need:

  //frag
  vec3 frag_pos = pass_vert_pos;
  vec3 frag_pos_world = pass_vert_pos_world;
  vec3 frag_normal= normalize(pass_Normal);
  //material:
  vec3 specular_color = vec3(1.0,0.0,0.0);
  vec3 diffuse_color = vec3(texture(YourTexture, pass_TexCoord));
  float shininess = 30.0;

  //light source:
  vec3 light_pos =vec3(0,0,0);
  float light_intensity=0.1;
  vec3 light_color= vec3(1.0,1.0,1.0);// * ModelMatrix;
  vec3 ambient_color=0.1* diffuse_color;

  //derived
  vec3 light_ray_in= normalize(light_pos-frag_pos);

  //calculation
  float scaling =0.005;
  vec3 light_actual=light_color*(light_intensity)/(4*PI*pow(scaling*length(light_pos-frag_pos),2.0f));

  //full formular
  //ambient:
  vec3 color_result = ambient_color;

  //difuse
  float lambertian =max(dot (light_ray_in, frag_normal), 0.0);
  color_result += light_actual * lambertian * diffuse_color;

  //specular
  if(lambertian>0){
    vec3 viewDir = normalize(-frag_pos_world);
    vec3 halfDir = normalize(light_ray_in + viewDir);
    float specAngle = max(dot(halfDir, frag_normal), 0.0);
    float specular = pow(specAngle, shininess);
    color_result += light_actual* (specular * specular_color);
  }

  out_Color = vec4(color_result.x,color_result.y,color_result.z, 1.0);
}
