#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "texture_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>



ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,star_object{}
 ,orbit_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{2.0f, 2.0f, 5.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeLabels();
  initializeStars();
  initializePlanets();
  initializeGeometry();
//  std::cout << "init g" <<"\n";
  initializeShaderPrograms();
//  std::cout << "init sp" <<"\n";

}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}


void ApplicationSolar::initializeLabels(){
  std::string p = "planet";
  Labels.push_back(p);
  p = "stars";
  Labels.push_back(p);
  p = "orbits";
  Labels.push_back(p);
}



void ApplicationSolar::initializeFramebuffer(){
  glGenRenderbuffers(1, &rb_handle);
  glBindRenderbuffer(GL_RENDERBUFFER, rb_handle);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &framebuffer_tex_obj);
  glBindTexture(GL_TEXTURE_2D, framebuffer_tex_obj);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glGenFramebuffers(1, &fbo_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebuffer_tex_obj, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_handle);
  GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1 , draw_buffers);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) { std::cout << "Frambuffer läuft nicht " << '\n';}
}

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


void ApplicationSolar::render() const{
  renderStars();
  renderPlanets();

}

void ApplicationSolar::renderPlanets() const {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);
  //for(unsigned int i = 0; i < scene.getRoot()->getChildren("sun")->getChildrenList().size(); i++){
  for(auto i : scene.getRoot()->getChildren("sun")->getChildrenList()){
    Geometrynode *gn{dynamic_cast<Geometrynode*>(i)};
    if(gn){
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
      glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                         1, GL_FALSE, glm::value_ptr(normal_matrix));
      //bind the VAO to draw
      glBindVertexArray(planet_object.vertex_AO);
      //draw bound vertex array using bound shaderscene
      glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      //---------------texturepart begins
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, framebuffer_tex_obj);
      glUniform1i(m_shaders.at("planet").u_locs.at("Planets_Texture"), 0);

      //upload_planet_transforms(gn->getSize());
      //color_planets(gn->getColor());


      // bind the VAO to draw
      glBindVertexArray(planet_object.vertex_AO);
      // draw bound vertex array using bound shader
      glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      //-----------------

    }
 }
}

void ApplicationSolar::renderStars() const {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("stars").handle);
  glBindVertexArray(star_object.vertex_AO);
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

//string input to upload View und upload projection  mit stars orbit or planet input
void ApplicationSolar::uploadView(std::string const& object) {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at(object).u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  if(object == "planet"){
    glUniform1f(m_shaders.at("planet").u_locs.at("ShadingMethod"), shader_mode);
  }
}

void ApplicationSolar::uploadProjection(std::string const& object) {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at(object).u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  // bind shader to which to upload uniforms
  //stinginput fehlt noch!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  glUseProgram(m_shaders.at("stars").handle);
  for (auto i : Labels) {
    glUseProgram(m_shaders.at(i).handle);
    uploadView(i);
    uploadProjection(i);
  }
  //glUseProgram(m_shaders.at("orbits").handle);
  // upload uniform values to new locations
}
///////////////////////////// intialisation functions //initializeScene///////////////////////


void ApplicationSolar::initializeTextures(){
  for (auto i: solarsystem_planets_)
  {
  glActiveTexture(GL_TEXTURE0);
  GLuint texture_object;
  glGenTextures(1, &texture_object);
  glBindTexture(GL_TEXTURE_2D, texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, i.texture_data.channels, i.texture_data.width, i.texture_data.height, 0,
              i.texture_data.channels, i.texture_data.channel_type, i.texture_data.ptr());
  i.texture_object = texture_object;
  std::cout << "init textures" <<"\n";

  }
  /*
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &sky_sphere_tex_obj);
  glBindTexture(GL_TEXTURE_2D, sky_sphere_tex_obj);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, sky_sphere_texture.channels, sky_sphere_texture.width, sky_sphere_texture.height, 0,
              sky_sphere_texture.channels, sky_sphere_texture.channel_type, sky_sphere_texture.ptr());
  std::cout << "working?" <<"\n";
  */
}
GLuint ApplicationSolar::initTexObj(pixel_data tex_data_in){
  glActiveTexture(GL_TEXTURE0);
  GLuint texture_object;
  glGenTextures(1, &texture_object);
  glBindTexture(GL_TEXTURE_2D, texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, tex_data_in.channels, tex_data_in.width, tex_data_in.height, 0,
              tex_data_in.channels, tex_data_in.channel_type, tex_data_in.ptr());
  return texture_object;
  std::cout << "init textures obj" <<"\n";

}

void ApplicationSolar::loadTextures(){
}

void ApplicationSolar::initializePlanets(){
  glm::fvec3 planet_color= {0.7,0.0, 0.2};

  pixel_data Sky_tex = texture_loader::file("../resources/textures/sky_sphere.png");
  pixel_data Sun_tex = texture_loader::file("../resources/textures/sunmap.png");
  pixel_data Merkur_tex = texture_loader::file("../resources/textures/mercurymap.png");
  pixel_data Venus_tex = texture_loader::file("../resources/textures/venusmap.png");
  pixel_data Erde_tex = texture_loader::file("../resources/textures/earthmap1k.png");
  pixel_data Mond_tex = texture_loader::file("../resources/textures/moonmap1k.png");
  pixel_data Mars_tex = texture_loader::file("../resources/textures/marsmap1k.png");
  pixel_data Jupiter_tex = texture_loader::file("../resources/textures/jupitermap.png");
  pixel_data Saturn_tex = texture_loader::file("../resources/textures/saturnmap.png");
  pixel_data Uranus_tex = texture_loader::file("../resources/textures/uranusmap.png");
  pixel_data Neptun_tex = texture_loader::file("../resources/textures/neptunemap.png");

  solarsystem_planets_.push_back(Planet("Merkur", 10.383f, 3.012f, 0.0f, {0.7,0.0, 0.2}, Merkur_tex));
  solarsystem_planets_.push_back(Planet("Venus", 0.950f, 1.177f, 2.723f, {0.7,0.6, 0.2}, Venus_tex));
  solarsystem_planets_.push_back(Planet("Erde", 1.0f, 1.0f, 3.0f, {0.7,0.0, 0.22}, Erde_tex));
  solarsystem_planets_.push_back(Planet("Mars",0.583f, 0.53f, 4.524f, {0.7,0.2, 0.2}, Mars_tex));
  solarsystem_planets_.push_back(Planet("Jupiter", 10.97f, 0.084f, 5.2f, {0.7,0.5, 0.42}, Jupiter_tex));
  solarsystem_planets_.push_back(Planet("Saturn",9.14f, 0.0339f, 8.54f, {0.7,0.0, 0.2}, Saturn_tex));
  solarsystem_planets_.push_back(Planet("Uranus", 3.98f, 0.0119f, 10.19f, {0.7,0.3, 0.2}, Uranus_tex));
  solarsystem_planets_.push_back(Planet("Neptun", 3.87f, 0.006f, 12.1f, {0.7,0.0, 0.52}, Neptun_tex));

  initializeTextures();

  Pointlightnode* light = new Pointlightnode(50, glm::vec3{1.0, 1.0, 1.0});


  Node* RootNode      =  new Node("RootOfTheUniverse");
  scene    =  Scenegraph("solarsystem", RootNode);
  Geometrynode* sun   =  new Geometrynode("sun", 1.0f, 0.0f, 0.0f, {0.7,0.0, 0.52}, initTexObj(Sun_tex));

  RootNode->addChildren(sun);
    /*
      for(int i = 0; i < 8; i++)
      {
        Geometrynode* planet = new Geometrynode("planeten");
        planet->setGeometry(planet_model);
        sun->addChildren(planet);
      }
    */
  for(auto i : solarsystem_planets_)
  {
    Geometrynode* planet = new Geometrynode(i.name, i.size, i.rotation_speed, i.distance, i.color, i.texture_object);
    planet->setGeometry(planet_model);
    sun->addChildren(planet);
  }
  std::cout << "init planets" <<"\n";

}

// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/planet.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet.frag"}}});

  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});


  m_shaders.emplace("orbits", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});



  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet").u_locs["ShadingMethod"] = -1;
  m_shaders.at("planet").u_locs["Planet_Color"] = -1;
  m_shaders.at("planet").u_locs["Planets_Texture"] = -1;



  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;


  m_shaders.at("orbits").u_locs["ViewMatrix"] = -1;
  m_shaders.at("orbits").u_locs["ProjectionMatrix"] = -1;

}

// load models
void ApplicationSolar::initializeGeometry() {

  //-----------------Orbits------------------------
  orbit_model = model{m_orbits, (model::POSITION ), {0}};

  // generate vertex array object
  glGenVertexArrays(1, &orbit_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(orbit_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &orbit_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbit_model.data.size(), orbit_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::POSITION]);

   // generate generic buffer
  glGenBuffers(1, &orbit_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * orbit_model.indices.size(), orbit_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  orbit_object.draw_mode = GL_LINES;
  // transfer number of indices to model object
  orbit_object.num_elements = GLsizei(orbit_model.indices.size());


  //------------------Stars--------------------
  star_model = model{Stars, (model::POSITION + model::NORMAL), {0}};

  // generate vertex array object
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




  //-----------Planets--------------------------
  planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
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


}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    for (auto i : Labels) {
      uploadView(i);
    }
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    for (auto i : Labels) {
    uploadView(i);
    }
  }
  else if (key == GLFW_KEY_1  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    shader_mode = 1;
    for (auto i : Labels) {
    uploadView(i);
    }
  }
  else if (key == GLFW_KEY_2  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    shader_mode = 2;
    for (auto i : Labels) {
    uploadView(i);
    }
  }
  else if (key == GLFW_KEY_3  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    shader_mode = 3;
    for (auto i : Labels) {
    uploadView(i);
    }
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {

  // mouse handling
  //std::cout<<"mouse update: "<<pos_x<<" , "<<pos_y<<"\n";
  m_view_transform = glm::rotate(m_view_transform, float(pos_x)/500, glm::fvec3{0.0f, -1.0f, 0.0f});
  m_view_transform = glm::rotate(m_view_transform, float(pos_y)/500, glm::fvec3{1.0f, 0.0f, 0.0f});
  for (auto i : Labels) {
  uploadView(i);
  }
  //we always need to upload, because camera is center

}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  for (auto i : Labels) {
  uploadProjection(i);
  }
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
