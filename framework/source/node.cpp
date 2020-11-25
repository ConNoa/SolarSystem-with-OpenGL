#include "node.hpp"
#include <iostream>

Node::Node():
    parent_{},
    children_{std::list<Node*>()},
    name_{},
    path_{},
    depth_{0},
    localTransform_{},
    worldTransform_{}
{}


  Node::Node(string node_name):
      parent_{},
      children_{std::list<Node*>()},
      name_{node_name},
      path_{},
      depth_{0},
      localTransform_{},
      worldTransform_{}
  {}



Node* Node::getParent(){
    return parent_;
}

void Node::setParent(Node* parent){
    parent_ = parent;
}

Node* Node::getChildren(string name){
    for(auto const& child : this -> getChildrenList()){
        if(child -> getName() == name){
            return child;
        }
    }
    std::cout<<"ERROR: Could not find Child Node named: "<<name_<<"\n";
    Node* errornode = new Node("error");
    return errornode;
  }

std::list<Node*>& Node::getChildrenList(){
    return children_;
}

std::string Node::getName(){
    return name_;
}

std::string Node::getPath(){
    return path_;
}

int Node::getDepth(){
    return depth_;
}

glm::mat4 Node::getLocalTransform(){
    return localTransform_;
}

void Node::setLocalTransform(glm::mat4 local_transform_input){
    localTransform_ = local_transform_input;
}

glm::mat4 Node::getWorldTransform(){
    return worldTransform_;
}

void Node::setWorldTransform(glm::mat4 world_transf_input){
    worldTransform_ = world_transf_input;
}

void Node::addChildren(Node* child_new){
    children_.push_back(child_new);
}

Node* Node::removeChildren(string name){
    Node* child = getChildren(name);
    children_.remove(child);
    return child;
}
