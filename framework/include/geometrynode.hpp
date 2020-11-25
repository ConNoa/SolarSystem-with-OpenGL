#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <string>

class Geometrynode : public Node{

  public:
    Geometrynode();
    Geometrynode(std::string node_name);
    model&   getGeometry();
    void     setGeometry(model& model_input);

  private:
    model geometry_;
};

#endif
