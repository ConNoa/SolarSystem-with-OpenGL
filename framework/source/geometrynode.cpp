#include "geometrynode.hpp"
#include <iostream>

using namespace std;



Geometrynode::Geometrynode():
    Node{},
    geometry_{},
    size_{},
    rotation_speed_{},
    distance_{}
{}

Geometrynode::Geometrynode(string node_name):
    Node{node_name},
    geometry_{},
    size_{},
    rotation_speed_{},
    distance_{}
{}

Geometrynode::Geometrynode(string node_name, float size_in, float rotation_speed_in, float distance_in):
    Node{node_name},
    geometry_{},
    size_{size_in},
    rotation_speed_{rotation_speed_in},
    distance_{distance_in}
{
  //cout<<"geometrynode mit allen parametern konstruiert\n";
}



model& Geometrynode::getGeometry(){
  return geometry_;
}


void Geometrynode::setGeometry(model& model_input){
  geometry_ = model_input;
}

float Geometrynode::getDis(){
  return distance_;
}
