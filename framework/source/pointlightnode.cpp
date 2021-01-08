#include "pointlightnode.hpp"
#include <iostream>




Pointlightnode::Pointlightnode():
Node{},
lightIntensity_{},
pointColor_{}
{}

Pointlightnode::Pointlightnode(float intensity_input, glm::vec3 color_input):
Node{},
lightIntensity_{intensity_input},
pointColor_{color_input}
{  std::cout<<"created Pointlightnode\n";
}

void Pointlightnode::setLightNode(float intensity_in, glm::vec3 color_in){
  lightIntensity_ = intensity_in;
  pointColor_  = color_in;
}


float Pointlightnode::getIntensity(){
  return lightIntensity_;
}

glm::vec3 Pointlightnode::getColor(){
  return pointColor_;
}
