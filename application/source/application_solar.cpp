#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "texture_loader.hpp"


#include <glbinding/gl/gl.h>
#include <glm/gtx/string_cast.hpp>
// use gl definitions from glbinding
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <iostream>



ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,star_object{}
 ,orbit_object{}
 ,sky_object{}
 ,view_matrix_{glm::translate(glm::fmat4{((0.199485, 0.019970, -0.979681, 0.000000), (-0.118151, 0.992984, -0.003819, 0.000000), (0.972736, 0.116513, 0.200445, 0.000000), (10.000000, 2.000000, 0.000000, 1.000000))}
 ,glm::fvec3{10.0f, 2.0f, 0.0f})}
 ,projection_matrix_{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,shader_mode {3.0f}
   {
     test_image = texture_loader::file("../resources/textures/sky_sphere.png");

     char* f = "../resources/textures/front.png";
     char* ba = "../resources/textures/back.png";
     char* t = "../resources/textures/top.png";
     char* bo = "../resources/textures/bottom.png";
     char* le = "../resources/textures/left.png";
     char* ri = "../resources/textures/right.png";



     //create_cube_map(f, ba, t, bo, le, ri, &texcube_obj);
     box_coords = {-1,-1; 1,-1; 1,1; -1,1};



     glActiveTexture(GL_TEXTURE0);
     glGenTextures(1, &texture_object);
     glBindTexture(GL_TEXTURE_2D, texture_object);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexImage2D(GL_TEXTURE_2D, 0, test_image.channels, test_image.width, test_image.height, 0,
                 test_image.channels, test_image.channel_type, test_image.ptr());
           std::cout << "init test-tuexture" <<"\n";


      initializeLabels();
      initializeStars();
      loadTextures();
      initializeSkybox();
      initializePlanets();
      initializeGeometry();
      initializeShaderPrograms();
      //initializeFramebuffer();
    }


ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}


void ApplicationSolar::initializeLabels(){
  Labels.push_back("sky");
  Labels.push_back("planet");
//  Labels.push_back("stars");
  Labels.push_back("orbits");
}

// void ApplicationSolar::initializeFramebuffer(){
//   glGenRenderbuffers(1, &rb_handle);
//   glBindRenderbuffer(GL_RENDERBUFFER, rb_handle);
//   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
//
//   glActiveTexture(GL_TEXTURE0);
//   glGenTextures(1, &framebuffer_tex_obj);
//   glBindTexture(GL_TEXTURE_2D, framebuffer_tex_obj);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//
//   glGenFramebuffers(1, &fbo_handle);
//   glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
//   glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebuffer_tex_obj, 0);
//   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_handle);
//   GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
//   glDrawBuffers(1 , draw_buffers);
//   GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//   if (status != GL_FRAMEBUFFER_COMPLETE) { std::cout << "Frambuffer läuft nicht " << '\n';}
// }

void ApplicationSolar::initializeStars(){
  //Erstellung eines Sterne-Vektors mit positions und farbangaben
  Stars_num = 200;
  for (int i = 0; i < Stars_num; ++i)
  {
    float pos_x = float(rand()%20) - 10.0f;
    float pos_y = float(rand()%20) - 10.0f;
    float pos_z = float(rand()%20) - 10.0f;
    float r =  float(rand()%200);
    float g =  float(rand()%2);
    float b =  float(rand()%100);

    Stars.push_back(pos_x);
    Stars.push_back(pos_y);
    Stars.push_back(pos_z);
    Stars.push_back(r);
    Stars.push_back(g);
    Stars.push_back(b);
  }
}



void ApplicationSolar::create_cube_map(const char* front, const char* back, const char* top,
                      const char* bottom, const char* left,const char* right,GLuint* tex_cube) {
  // generate a cube-map texture to hold all the sides
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, tex_cube);

  // load each image and copy into a side of the cube-map texture
  load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
  load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
  load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
  load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
  load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
  load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
  // format cube map texture
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool ApplicationSolar::load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  auto temp = texture_loader::file(file_name);
  // copy image data into 'target' side of cube map
  glTexImage2D(GL_TEXTURE_2D, 0, temp.channels, temp.width, temp.height, 0,
              temp.channels, temp.channel_type, temp.ptr());
  return true;
}






void ApplicationSolar::initializeSkybox(){
  // float cubemap_coords[] = {
  //       -10.0f,  10.0f, -10.0f,
  //       -10.0f, -10.0f, -10.0f,
  //        10.0f, -10.0f, -10.0f,
  //        10.0f, -10.0f, -10.0f,
  //        10.0f,  10.0f, -10.0f,
  //       -10.0f,  10.0f, -10.0f,
  //
  //       -10.0f, -10.0f,  10.0f,
  //       -10.0f, -10.0f, -10.0f,
  //       -10.0f,  10.0f, -10.0f,
  //       -10.0f,  10.0f, -10.0f,
  //       -10.0f,  10.0f,  10.0f,
  //       -10.0f, -10.0f,  10.0f,
  //
  //        10.0f, -10.0f, -10.0f,
  //        10.0f, -10.0f,  10.0f,
  //        10.0f,  10.0f,  10.0f,
  //        10.0f,  10.0f,  10.0f,
  //        10.0f,  10.0f, -10.0f,
  //        10.0f, -10.0f, -10.0f,
  //
  //       -10.0f, -10.0f,  10.0f,
  //       -10.0f,  10.0f,  10.0f,
  //        10.0f,  10.0f,  10.0f,
  //        10.0f,  10.0f,  10.0f,
  //        10.0f, -10.0f,  10.0f,
  //       -10.0f, -10.0f,  10.0f,
  //
  //       -10.0f,  10.0f, -10.0f,
  //        10.0f,  10.0f, -10.0f,
  //        10.0f,  10.0f,  10.0f,
  //        10.0f,  10.0f,  10.0f,
  //       -10.0f,  10.0f,  10.0f,
  //       -10.0f,  10.0f, -10.0f,
  //
  //       -10.0f, -10.0f, -10.0f,
  //       -10.0f, -10.0f,  10.0f,
  //        10.0f, -10.0f, -10.0f,
  //        10.0f, -10.0f, -10.0f,
  //       -10.0f, -10.0f,  10.0f,
  //        10.0f, -10.0f,  10.0f};

  model sky_model = model_loader::obj(m_resource_path + "models/skybox.obj", model::NORMAL);
  // generate vertex array object
  glGenVertexArrays(1, &sky_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(sky_object.vertex_AO);
  // generate generic buffer
  glGenBuffers(1, &sky_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, sky_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sky_model.data.size() * sizeof(float), sky_model.data.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, skybox_model.vertex_bytes, skybox_model.offsets[model::POSITION]);
   // activate third attribute on gpu
  // glEnableVertexAttribArray(1);
  // // third attribute is 2 floats with no offset & stride
  // glVertexAttribPointer(1, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, sky_model.vertex_bytes, sky_model.offsets[model::TEXCOORD]);
  // // activate first attribute on gpu
  // glEnableVertexAttribArray(0);
  // // first attribute is 3 floats with no offset & stride
  // glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, sky_model.vertex_bytes, sky_model.offsets[model::POSITION]);
  // // activate second attribute on gpu
  // glEnableVertexAttribArray(1);
  // // first attribute is 3 floats with no offset & stride
  // glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, sky_model.vertex_bytes, sky_model.offsets[model::POSITION]);
  // generate generic buffer
  glGenBuffers(1, &sky_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * sky_model.indices.size(), sky_model.indices.data(), GL_STATIC_DRAW);
  //store type of primitive to draw
  sky_object.draw_mode = GL_TRIANGLE_STRIP;
  // transfer number of indices to model object
  sky_object.num_elements = GLsizei(sky_model.indices.size());
}



void ApplicationSolar::initializeTextures(std::vector<std::pair<std::string, pixel_data>> tex_files){
  for (auto i: tex_files)
  {
    std::string tmp_string = i.first;
    pixel_data tmp_px_dt = i.second;
    //    std::vector<pair<std::string, GLuint>> tex_handle;
    GLuint tex_handle;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, tmp_px_dt.channels, tmp_px_dt.width, tmp_px_dt.height, 0,
                tmp_px_dt.channels, tmp_px_dt.channel_type, tmp_px_dt.ptr());

    handles_.insert(std::make_pair(tmp_string, tex_handle));
    }
    // glActiveTexture(GL_TEXTURE1);
    // glGenTextures(1, &sky_sphere_tex_obj);
    // glBindTexture(GL_TEXTURE_2D, sky_sphere_tex_obj);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, sky_sphere_texture.channels, sky_sphere_texture.width, sky_sphere_texture.height, 0,
    //             sky_sphere_texture.channels, sky_sphere_texture.channel_type, sky_sphere_texture.ptr());

}

GLuint ApplicationSolar::initTexObj(pixel_data tex_data_in){
  glActiveTexture(GL_TEXTURE0);
  GLuint tex_handle;
  glGenTextures(1, &tex_handle);
  glBindTexture(GL_TEXTURE_2D, tex_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, tex_data_in.channels, tex_data_in.width, tex_data_in.height, 0,
              tex_data_in.channels, tex_data_in.channel_type, tex_data_in.ptr());
              std::cout << "init callme cbabytextures sun" <<"\n";
  return tex_handle;
}

void ApplicationSolar::loadTextures(){
  std::vector<pair<std::string, pixel_data>> tex_files;
    tex_files.push_back({"Sky", texture_loader::file("../resources/textures/sky_sphere.png")});
    tex_files.push_back({"Sun", texture_loader::file("../resources/textures/sunmap.png")});
    tex_files.push_back({"Merkur", texture_loader::file("../resources/textures/mercurymap.png")});
    tex_files.push_back({"Venus", texture_loader::file("../resources/textures/venusmap.png")});
    tex_files.push_back({"Erde", texture_loader::file("../resources/textures/earthmap1k.png")});
    tex_files.push_back({"Mond", texture_loader::file("../resources/textures/moonmap1k.png")});
    tex_files.push_back({"Mars", texture_loader::file("../resources/textures/marsmap1k.png")});
    tex_files.push_back({"Jupiter", texture_loader::file("../resources/textures/jupitermap.png")});
    tex_files.push_back({"Saturn", texture_loader::file("../resources/textures/saturnmap.png")});
    tex_files.push_back({"Uranus", texture_loader::file("../resources/textures/uranusmap.png")});
    tex_files.push_back({"Neptun", texture_loader::file("../resources/textures/neptunemap.png")});

    initializeTextures(tex_files);

    sky_sphere_texture = texture_loader::file("../resources/textures/sky_sphere.png");

}

void ApplicationSolar::initializePlanets(){
  glm::fvec3 planet_color= {0.7,0.0, 0.2};

  solarsystem_planets_.push_back(Planet("Merkur", 10.383f, 3.012f, 1.0f, {0.7,0.0, 0.2}));
  solarsystem_planets_.push_back(Planet("Venus", 0.950f, 1.177f, 2.723f, {0.7,0.6, 0.2}));
  solarsystem_planets_.push_back(Planet("Erde", 1.0f, 1.0f, 3.0f, {0.7,0.0, 0.22}));
  solarsystem_planets_.push_back(Planet("Mars",0.583f, 0.53f, 4.524f, {0.7,0.2, 0.2}));
  solarsystem_planets_.push_back(Planet("Jupiter", 10.97f, 0.084f, 5.2f, {0.7,0.5, 0.42}));
  solarsystem_planets_.push_back(Planet("Saturn",9.14f, 0.0339f, 8.54f, {0.7,0.0, 0.2}));
  solarsystem_planets_.push_back(Planet("Uranus", 3.98f, 0.0119f, 10.19f, {0.7,0.3, 0.2}));
  solarsystem_planets_.push_back(Planet("Neptun", 3.87f, 0.006f, 12.1f, {0.7,0.0, 0.52}));

  Pointlightnode* light = new Pointlightnode(50,glm::vec3{1.0, 1.0, 1.0});

  Node* RootNode      =  new Node("RootOfTheUniverse");
  scene    =  Scenegraph("solarsystem", RootNode);
  auto sun_handle =  handles_.find("Sun")->second;
  Geometrynode* sun   =  new Geometrynode("sun", 0.10f, 0.0f, 0.0f, {0.7,0.0, 0.52}, sun_handle);

  RootNode->addChildren(sun);

  for(int i = 0; i < solarsystem_planets_.size(); i++){
    Geometrynode* planet = new Geometrynode(solarsystem_planets_[i].name,
                                            solarsystem_planets_[i].size,
                                            solarsystem_planets_[i].rotation_speed,
                                            solarsystem_planets_[i].distance,
                                            solarsystem_planets_[i].color,
                                            handles_.find(solarsystem_planets_[i].name)->second);
    planet->setGeometry(planet_model);
    sun->addChildren(planet);
  }
}
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/planet.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet.frag"}}});
    // request uniform locations for shader program
       m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
       m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
       m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
       m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
       m_shaders.at("planet").u_locs["ShadingMethod"] = -1;
       m_shaders.at("planet").u_locs["Planet_Color"] = -1;
       m_shaders.at("planet").u_locs["Planets_Texture"] = -1;

  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
        m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
        m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("orbits", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
       m_shaders.at("orbits").u_locs["ViewMatrix"] = -1;
       m_shaders.at("orbits").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("sky", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/skybox.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});
      // request uniform locations for shader program
          m_shaders.at("sky").u_locs["ViewMatrix"] = -1;
          m_shaders.at("sky").u_locs["ProjectionMatrix"] = -1;
          m_shaders.at("sky").u_locs["ColorTex"] = -1;
  //
  // m_shaders.emplace("quad", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/quad.vert"},
  //                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/quad.frag"}}});
  //         m_shaders.at("quad").u_locs["ColorTex"] = -1;
  //         m_shaders.at("quad").u_locs["grey_bool"] = -1;
  //         m_shaders.at("quad").u_locs["horizont_bool"] = -1;
  //         m_shaders.at("quad").u_locs["vert_bool"] = -1;
  //         //m_shaders.at("quad").u_locs["blur_bool"] = -1;

}

// load models
void ApplicationSolar::initializeGeometry() {

  // //-----------------Orbits------------------------
  // orbit_model = model{m_orbits, (model::POSITION ), {0}};
  //
  // // generate vertex array object
  // glGenVertexArrays(1, &orbit_object.vertex_AO);
  // // bind the array for attaching buffers
  // glBindVertexArray(orbit_object.vertex_AO);
  //
  // // generate generic buffer
  // glGenBuffers(1, &orbit_object.vertex_BO);
  // // bind this as an vertex array buffer containing all attributes
  // glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
  // // configure currently bound array buffer
  // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbit_model.data.size(), orbit_model.data.data(), GL_STATIC_DRAW);
  //
  // // activate first attribute on gpu
  // glEnableVertexAttribArray(0);
  // // first attribute is 3 floats with no offset & stride
  // glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::POSITION]);
  //  // generate generic buffer
  // glGenBuffers(1, &orbit_object.element_BO);
  // // bind this as an vertex array buffer containing all attributes
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
  // // configure currently bound array buffer
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * orbit_model.indices.size(), orbit_model.indices.data(), GL_STATIC_DRAW);
  // // store type of primitive to draw
  // orbit_object.draw_mode = GL_LINES;
  // // transfer number of indices to model object
  // orbit_object.num_elements = GLsizei(orbit_model.indices.size());


  //-----------Planets--------------------------
  planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
  // generate vertex array objectgl mirrored repeat
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);
  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(2);
   // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

  // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);
  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  planet_object.num_elements = GLsizei(planet_model.indices.size());

  //------------------Stars--------------------
  star_model = model{Stars, (model::POSITION + model::NORMAL), {0}};

  // generate vertex array objectgl mirrored repeat
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * star_model.data.size(), star_model.data.data(), GL_STATIC_DRAW);
  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * star_model.indices.size(), star_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object
  star_object.num_elements = GLsizei(star_model.indices.size());



  // //-----Skybox------------
  // //sky_model = model_loader::obj(m_resource_path + "models/skybox.obj", model::NORMAL);
  // // generate vertex array object
  // glGenVertexArrays(1, &sky_object.vertex_AO);
  // // bind the array for attaching buffers
  // glBindVertexArray(sky_object.vertex_AO);
  // // generate generic buffer
  // glGenBuffers(1, &sky_object.vertex_BO);
  // // bind this as an vertex array buffer containing all attributes
  // glBindBuffer(GL_ARRAY_BUFFER, sky_object.vertex_BO);
  // // configure currently bound array buffer
  // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Quad_vector.size(), Quad_vector.data(), GL_STATIC_DRAW);
  // // activate first attribute on gpu
  // glEnableVertexAttribArray(0);
  // // first attribute is 3 floats with no offset & stride
  // glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, sky_model.vertex_bytes, sky_model.offsets[model::POSITION]);
  // // generate generic buffer
  // glGenBuffers(1, &sky_object.element_BO);
  // // bind this as an vertex array buffer containing all attributes
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_object.element_BO);
  // // configure currently bound array buffer
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * sky_model.indices.size(), sky_model.indices.data(), GL_STATIC_DRAW);
  // // store type of primitive to draw
  // sky_object.draw_mode = GL_TRIANGLE_STRIP;
  // // transfer number of indices to model object
  // sky_object.num_elements = GLsizei(Quad_vector.size()/5);
  // std::cout<<"sky-geo-int-complete"<<"\n";

}

void ApplicationSolar::render() const{
  renderSky();
//  renderStars();
  renderSolarsystem();
}


void ApplicationSolar::initializeSkyBoxTex(){
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &skybox.handle);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.handle);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  for(unsigned int idx = 0; idx < skybox_textures.size(); ++idx){
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, skybox_textures[idx].m_pixelData.channels, (GLsizei)skybox_textures[idx].m_pixelData.width,
                (GLsizei)skybox_textures[idx].m_pixelData.height, 0, skybox_textures[idx].m_pixelData.channels,
                 skybox_textures[idx].m_pixelData.channel_type, skybox_textures[idx].m_pixelData.ptr());
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
}



void ApplicationSolar::renderSky() const{
  glDepthMask(GL_FALSE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glUseProgram(m_shaders.at("sky").handle);
  glActiveTexture(GL_TEXTURE0);

  int color_sampler_location = glGetUniformLocation(m_shaders.at("sky").handle, "ColorTex");
  glBindTexture(GL_TEXTURE_CUBE_MAP, texcube_obj);
//  glBindTexture(GL_TEXTURE_2D, texture_object);
  glUniform1i(color_sampler_location,0);

  glBindVertexArray(sky_object.vertex_AO);
  glDrawElements(sky_object.draw_mode, sky_object.num_elements, model::INDEX.type, NULL);

  glDepthMask(GL_TRUE);
}

void ApplicationSolar::renderStars() const {
  // bind shader to upload uniforms
  glm::fmat4 view_matrix = glm::inverse(view_matrix_);
  glUseProgram(m_shaders.at("stars").handle);
  glBindVertexArray(star_object.vertex_AO);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                    1, GL_FALSE, glm::value_ptr(projection_matrix_));
  glPointSize(3.0);
  glDrawArrays(star_object.draw_mode, 0, (int)Stars.size());
}

void ApplicationSolar::renderOrbits() const {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("orbits").handle);
  glBindVertexArray(orbit_object.vertex_AO);
  glLineWidth(40.0);
  glDrawArrays(orbit_object.draw_mode, 0, (int)m_orbits.size());
}

void ApplicationSolar::renderSolarsystem() const{
  auto k = scene.getRoot()->getChildren("sun");
  Geometrynode *gn{dynamic_cast<Geometrynode*>(k)};
  if(gn)renderPlanets(gn);
  for(auto i : scene.getRoot()->getChildren("sun")->getChildrenList()){
    Geometrynode *gn{dynamic_cast<Geometrynode*>(i)};
    if(gn)renderPlanets(gn);
  }
}

void ApplicationSolar::renderPlanets(Geometrynode* gn) const {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);
    glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()*gn->getRot_speed()), glm::fvec3{0.0f, 1.0f, 0.0f});
    //glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()*solarsystem_planets_[i].rotation_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
    //model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f*solarsystem_planets_[i].distance});
    float act_dis = gn->getDis();
    model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -3.0f*act_dis});
    //integrated part
    model_matrix = glm::rotate(model_matrix, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
    //glm::fvec3 scale {(9-i)/4, (9-i)/4, (9-i)/4};
    float act_size = 4*gn->getSize();
    glm::fvec3 scale {act_size, act_size, act_size};
    //model_matrix = glm::scale(model_matrix, scale);
    //glm::fvec3 planet_color= {0.7,0.0, 0.2*i};
    glm::fvec3 planet_color= gn->getColor();
    //integrated parts
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniform3f(m_shaders.at("planet").u_locs.at("Planet_Color"),planet_color.x, planet_color.y, planet_color.z);
    //extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(view_matrix_) * model_matrix);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));
    glUniform1f(m_shaders.at("planet").u_locs.at("ShadingMethod"), shader_mode);
    //bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);
    //draw bound vertex array using bound shaderscene
    //Texrepart begins
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gn->getTexture());
    glUniform1i(m_shaders.at("planet").u_locs.at("Planets_Texture"), 0);
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  }

//string input to upload View und upload projection  mit stars orbit or planet input
void ApplicationSolar::uploadView(std::string const& object) {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 object_view_matrix = glm::inverse(view_matrix_);
  // upload matrix to gpum_view_transform
  glUseProgram(m_shaders.at(object).handle);

  glUniformMatrix4fv(m_shaders.at(object).u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(object_view_matrix));

}

void ApplicationSolar::uploadProjection(std::string const& object) {
  glUseProgram(m_shaders.at(object).handle);

  glUniformMatrix4fv(m_shaders.at(object).u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(projection_matrix_));
}

void ApplicationSolar::uploadUniforms() {
  for (auto i : Labels) {
    uploadView(i);
    uploadProjection(i);
  }
}

///////////////////////////// callback functions for window events ////////////
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    view_matrix_ = glm::translate(view_matrix_, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadUniforms();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    view_matrix_ = glm::translate(view_matrix_, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadUniforms();
  }
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    view_matrix_ = glm::translate(view_matrix_, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadUniforms();
  }
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    view_matrix_ = glm::translate(view_matrix_, glm::fvec3{0.1f, 0.0f, 0.0f});
    uploadUniforms();
  }
  else if (key == GLFW_KEY_V  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    std::cout << "View mat: "<<"\n";
std::cout << glm::to_string(view_matrix_) << std::endl;
  }
  else if (key == GLFW_KEY_P  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    std::cout << "Projection mat: " <<"\n";
std::cout << glm::to_string(projection_matrix_) << std::endl;
  }
  else if (key == GLFW_KEY_1  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    shader_mode = 1;
  }
  else if (key == GLFW_KEY_2  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    shader_mode = 2;
  }
  else if (key == GLFW_KEY_3  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    shader_mode = 3;
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  //std::cout<<"mouse update: "<<pos_x<<" , "<<pos_y<<"\n";
  view_matrix_ = glm::rotate(view_matrix_, float(pos_x)/500, glm::fvec3{0.0f, -1.0f, 0.0f});
  view_matrix_ = glm::rotate(view_matrix_, -float(pos_y)/500, glm::fvec3{1.0f, 0.0f, 0.0f});
  for (auto i : Labels) {
  uploadView(i);
  }
  //we always need to upload, because camera is center
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  projection_matrix_ = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  for (auto i : Labels) {
  uploadProjection(i);
  }
}

// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
