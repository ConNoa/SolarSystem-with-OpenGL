#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scenegraph.hpp"

#include "node.hpp"
#include "geometrynode.hpp"
#include "cameranode.hpp"
#include "model.hpp"

#define PI 3.1415926


// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();


  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;

  void renderPlanets() const;

  void renderStars() const;

  void renderOrbits() const;

 protected:
   //added  Stars and Orbits
  void initializeStars();

  void initializeOrbits();

   // initialize all Planets and the Moons
  void initializeScene();

  void initializeShaderPrograms();

  void initializeGeometry();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object orbit_object;

  Scenegraph scene;
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  model planet_model;
  model star_model;
  model orbit_model;

  int Stars_num;

  std::vector<float> Stars;
  std::vector<float> m_orbits;


};

#endif