#include "cameranode.hpp"

using namespace std;
using namespace glm;


Cameranode::Cameranode():
  Node{},
  isPerspective_{},
  isEnabled_{},
  projectionMatrix_{}
  {}

Cameranode::~Cameranode(){}

bool Cameranode::getPerspective(){
  return isPerspective_;
}

bool Cameranode::getEnabled(){
  return isEnabled_;
}

void Cameranode::setEnabled(bool is_or_not){
  isEnabled_ = is_or_not;
}


mat4& Cameranode::getProjectionMatrix(){
  return projectionMatrix_;
}

void Cameranode::setProjectionMatrix(mat4& p_mat_input){
  projectionMatrix_ = p_mat_input;
}
