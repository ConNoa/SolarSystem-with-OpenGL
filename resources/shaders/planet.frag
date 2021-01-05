#version 150


in vec3 pass_Normal_world;
in vec3 pass_Normal_view;
in vec3 pass_vert_Pos;
in vec3 pass_vert_Pos_view;

in vec3 pass_Color;


out vec4 out_Color;


const vec3 Sun = vec3(0.0, 0.0, 0.0);

//indirect light incoming
vec3 ambient_color = pass_Color;

//diffusely reflected light
const vec3 diffuse_color = vec3(1.0, 1.0, 1.0);

//Reflection of Light to viewer
const vec3 specular_color = vec3(1.0, 1.0, 1.0);

const float amb_fac = 0.5;
const float dif_fac = 0.5;
const float spec_fac = 0.9;

//Shininess, woooohoooo
const float shininess = 100.0;




void main() {
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

  resulting_color = ambient_color*amb_fac + lambertian*diffuse_color*dif_fac + specular_color*specular*spec_fac;

  out_Color = vec4(resulting_color, 1.0);

}

/*

precision mediump float;

in vec3 normalInterp;
in vec3 vertPos;

uniform int mode;

const vec3 lightPos = vec3(1.0, 1.0, 1.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float lightPower = 40.0;
const vec3 ambientColor = vec3(0.1, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

void main() {

  vec3 normal = normalize(normalInterp);
  vec3 lightDir = lightPos - vertPos;
  float distance = length(lightDir);
  distance = distance * distance;
  lightDir = normalize(lightDir);

  float lambertian = max(dot(lightDir, normal), 0.0);
  float specular = 0.0;

  if (lambertian > 0.0) {

    vec3 viewDir = normalize(-vertPos);

    // this is blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    specular = pow(specAngle, shininess);

    // this is phong (for comparison)
    if (mode == 2) {
      vec3 reflectDir = reflect(-lightDir, normal);
      specAngle = max(dot(reflectDir, viewDir), 0.0);
      // note that the exponent is different here
      specular = pow(specAngle, shininess/4.0);
    }
  }
  vec3 colorLinear = ambientColor +
                     diffuseColor * lambertian * lightColor * lightPower / distance +
                     specColor * specular * lightColor * lightPower / distance;
  // apply gamma correction (assume ambientColor, diffuseColor and specColor
  // have been linearized, i.e. have no gamma correction in them)
  vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0 / screenGamma));
  // use the gamma corrected color in the fragment
  gl_FragColor = vec4(colorGammaCorrected, 1.0);
}
*/
