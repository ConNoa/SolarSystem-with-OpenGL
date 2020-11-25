#include "scenegraph.hpp"

using namespace std;


Scenegraph::Scenegraph():
  name_{},
  root_{}
  {}



Scenegraph::Scenegraph(string name, Node* root_node):
  name_{name},
  root_{root_node}
  {}



Scenegraph::~Scenegraph(){}


string Scenegraph::getName(){
  return name_;
}

Node* Scenegraph::getRoot(){
  return root_;
}


void Scenegraph::setName(string const& scene_name){
    name_ = scene_name;
}


void Scenegraph::setRoot(Node* root_node){
    root_ = root_node;
}
