#ifndef NODE_HPP
#define NODE_HPP

#include<string>
#include<list>
#include<glm/glm.hpp>
#include<memory>

#include "structs.hpp"

using namespace std;
using namespace glm;


//Superclass
class Node{
    public:

        Node();
        ~Node();

        shared_ptr<Node>            getParent();
        void                        setParent(shared_ptr<Node> parent);
        shared_ptr<Node>            getChildren(string name);
        list<shared_ptr<Node>>      getChildrenList();
        string                      getName();
        string                      getPath();
        int                         getDepth();
        mat4                        getLocalTransform();
        void                        setLocalTransform(mat4 local_transf_input);
        mat4                        getWorldTransform();
        void                        setWorldTransform(mat4 world_transf_input);
        void                        addChildren(shared_ptr<Node> child);
        std::shared_ptr<Node>       removeChildren(string name);


    private:

        shared_ptr<Node>            parent_;
        list<shared_ptr<Node>>      children_;
        string                      name_;
        string                      path_;
        int                         depth_;
        mat4                        localTransform_;
        mat4                        worldTransform_;

};


#endif
