#include "scenegraph.hpp"


Scenegraph::Scenegraph():
  scene_name_{},
  root_{}
  {}



Scenegraph::Scenegraph(std::string const& name):
  scene_name_{name},
  root_{}
  {}



Scenegraph::~Scenegraph(){}
