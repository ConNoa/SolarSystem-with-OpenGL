#ifndef NODE_HPP
#define NODE_HPP

#include<string>
#include<list>
#include<glm/glm.hpp>
#include<memory>

#include "structs.hpp"

using namespace std;


//Superclass
class Node{
    public:

        Node();
        Node(string node_name);

        Node*                       getParent();
        void                        setParent(Node* parent);

        Node*                       getChildren(string name);
        list<Node*>&                getChildrenList();

        string                      getName();
        string                      getPath();
        int                         getDepth();

        glm::mat4                   getLocalTransform();
        void                        setLocalTransform(glm::mat4 local_transf_input);


        glm::mat4                   getWorldTransform();
        void                        setWorldTransform(glm::mat4 world_transf_input);


        void                        addChildren(Node* child_new);
        Node*                       removeChildren(string name);


    private:

        Node*                       parent_;
        list<Node*>                 children_;
        string                      name_;
        string                      path_;
        int                         depth_;
        glm::mat4                   localTransform_;
        glm::mat4                   worldTransform_;

};


#endif
