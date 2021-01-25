#include "geometrynode.hpp"
#include <iostream>

using namespace std;



Geometrynode::Geometrynode():
    Node{},
    geometry_{},
    size_{},
    rotation_speed_{},
    distance_{},
    color_{},
    texture_{}
{}

Geometrynode::Geometrynode(string node_name):
    Node{node_name},
    geometry_{},
    size_{},
    rotation_speed_{},
    distance_{},
    color_{},
    texture_{}
{}

Geometrynode::Geometrynode(string node_name, float size_in, float rotation_speed_in, float distance_in, glm::vec3 color_in, GLuint tex_in):
    Node{node_name},
    geometry_{},
    size_{size_in},
    rotation_speed_{rotation_speed_in},
    distance_{distance_in},
    color_{color_in},
    texture_{tex_in}
{
  //cout<<"geometrynode mit allen parametern konstruiert\n";
}



model& Geometrynode::getGeometry(){
  return geometry_;
}


void Geometrynode::setGeometry(model& model_input){
  geometry_ = model_input;
}

float Geometrynode::getSize(){
  return size_;
}

float Geometrynode::getRot_speed(){
  return rotation_speed_;
}

float Geometrynode::getDis(){
  return distance_;
}

glm::vec3 Geometrynode::getColor(){
  return color_;
}

GLuint Geometrynode::getTexture(){
  return texture_;
}
