#include "geometrynode.hpp"

using namespace std;



Geometrynode::Geometrynode():
    Node{},
    geometry_{}
{}

Geometrynode::Geometrynode(string node_name):
    Node{node_name},
    geometry_{}
{}


model& Geometrynode::getGeometry(){
  return geometry_;
}


void Geometrynode::setGeometry(model& model_input){
  geometry_ = model_input;
}
