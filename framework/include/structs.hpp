#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <map>
#include <glbinding/gl/gl.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "pixel_data.hpp"
// use gl definitions from glbinding
using namespace gl;

//Planets for the Solarsystem
struct Planet{

  Planet():
  name("planet"),
  size(1.0f),
  rotation_speed(1.0f),
  distance(1.0f), // to origin
  color({0.3f,0.5f,0.7f}),
  texture_data(),
  texture_object()
  {}


      Planet(std::string planet_name, float size_in, float rotation_speed_in, float distance_in, glm::vec3 color_in)
                  : name(planet_name), size(size_in), rotation_speed(rotation_speed_in), distance(distance_in), color(color_in){}
      std::string name;
      float size;
      float rotation_speed;
      float distance;
      glm::vec3 color;
      pixel_data texture_data;
      GLuint texture_object;

};

// gpu representation of model
struct model_object {
  // vertex array object
  GLuint vertex_AO = 0;
  // vertex buffer object
  GLuint vertex_BO = 0;
  // index buffer object
  GLuint element_BO = 0;
  // primitive type to draw
  GLenum draw_mode = GL_NONE;
  // indices number, if EBO exists
  GLsizei num_elements = 0;
};

// gpu representation of texture
struct texture_object {
  // handle of texture object
  GLuint handle = 0;
  // binding point
  GLenum target = GL_NONE;
};

struct texture{
  texture(std::string name, pixel_data pixelData)
    :name_{name}
    ,pixelDats_{pixelData}
    {}

    std::string name_;
    pixel_data pixelDats_;
};


// shader handle and uniform storage
struct shader_program {
  shader_program(std::map<GLenum, std::string> paths)
   :shader_paths{paths}
   ,handle{0}
   {}

  // paths to shader sources
  std::map<GLenum, std::string> shader_paths;
  // object handle
  GLuint handle;
  // uniform locations mapped to name
  std::map<std::string, GLint> u_locs{};
};
#endif
