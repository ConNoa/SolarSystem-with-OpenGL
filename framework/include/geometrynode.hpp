#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <string>

class Geometrynode : public Node{

  public:
    Geometrynode();
    Geometrynode(std::string node_name);
    Geometrynode(std::string node_name, float size_in, float rotation_speed_in, float distance_in, glm::vec3 color_in);

    model&   getGeometry();
    void     setGeometry(model& model_input);


    float   getSize();
    float   getRot_speed();
    float   getDis();
    glm::vec3 getColor();

    float size_;
    float rotation_speed_;
    float distance_;
    glm::vec3 color_;
  private:
    model geometry_;

};

#endif
