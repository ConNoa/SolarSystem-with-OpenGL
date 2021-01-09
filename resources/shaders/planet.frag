#version 150


in vec3 pass_Normal_world;
in vec3 pass_Normal_view;
in vec3 pass_vert_Pos;
in vec3 pass_vert_Pos_view;
in float pass_ShadingMethod;

in vec3 pass_Color;


out vec4 out_Color;


const vec3 Sun = vec3(0.0, 0.0, 0.0);

//indirect light incoming
vec3 ambient_color = pass_Color;

//diffusely reflected light
const vec3 diffuse_color = vec3(1.0, 1.0, 1.0);

//Reflection of Light to viewer
const vec3 specular_color = vec3(1.0, 1.0, 1.0);

const float amb_fac = 0.4;
const float dif_fac = 0.2;
const float spec_fac = 0.9;

//Shininess, woooohoooo
const float shininess = 8.0;




void main() {
if(pass_ShadingMethod == 1){

    vec3 normal = normalize(pass_Normal_world);
    vec3 normal_view = normalize(pass_Normal_view);
    vec3 light_Direction = normalize(Sun - pass_vert_Pos);

    vec3 resulting_color;

    float lambertian = max(dot(light_Direction, normal), 0.0);

    float specular = 0.0;

    if(lambertian > 0.0){
        vec3 viewDir = normalize(-pass_vert_Pos);
        vec3 halfDir = normalize(light_Direction + viewDir);
        float specular_Angle = max(dot(halfDir, normal), 0.0);
        specular = pow(specular_Angle, shininess);
      }

    resulting_color = ambient_color + lambertian*diffuse_color + specular_color*specular*spec_fac;
//ouColor = ambientTerm + ((lightIntensity*lightColor)/4*PI*(lightPos - fragPos)^2) * (lambertianTerm + specularTerm)
    out_Color = vec4(resulting_color, 1.0);
  }

if(pass_ShadingMethod == 2){

  vec3 viewDir_sun = normalize(-pass_vert_Pos);

  vec3 normal = normalize(pass_Normal_world);
  vec3 normal_view = -normalize(pass_Normal_view);

  vec3 light_Direction = normalize(Sun - pass_vert_Pos);
  vec3 resulting_color;

  vec3 halfDir = normalize(light_Direction + viewDir_sun);
  float specular = 0.0;


  float lambertian = max(dot(light_Direction, normal), 0.0);

  if(lambertian>0){
    if(lambertian>0.75){
      lambertian = 1;
    }
    else if(lambertian > 0.5){
      lambertian = 0.75;
    }
    else if(lambertian > 0.25){
      lambertian = 0.5;
    }
    else if (lambertian > 0.0){
      lambertian = 0.25;
    }
    float specular_Angle = max(dot(halfDir, normal), 0.0);
    specular = pow(specular_Angle, shininess);

  }
  float view_angle = dot(pass_vert_Pos_view, normal_view);

  if(abs(view_angle)<0.95){
    //colored outline
    resulting_color = vec3(0,0.5,1);
  }
  else{
    resulting_color = ambient_color+lambertian*diffuse_color+specular_color*specular;
  }

  out_Color = vec4(resulting_color, 1.0);

}

}
