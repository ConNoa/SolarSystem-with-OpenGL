#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include <string>
#include "node.hpp"




class Scenegraph{
  public:

    Scenegraph();
    Scenegraph(std::string name, Node* root_node);
    ~Scenegraph();


    std::string              getName() const;
    Node*                    getRoot() const;
    std::string              printGraph();


  private:

    void                    setName(std::string const& scene_name);
    void                    setRoot(Node*);


    std::string   name_;
    Node*         root_;

};

#endif
