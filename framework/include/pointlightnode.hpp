#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include "node.hpp"
#include <glm/glm.hpp>


class Pointlightnode : public Node {

  public:

    Pointlightnode();
    Pointlightnode(float intensity_input, glm::vec3 color_input);
    ~Pointlightnode();


    void setLightNode(float intensity, glm::vec3 color);

  private:

    glm::vec3     pointColor_;
    float         lightIntensity_;

};

#endif
