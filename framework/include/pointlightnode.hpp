#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include "node.hpp"
#include <glm/glm.hpp>


class Pointlightnode : public Node {

  public:

    Pointlightnode();
    Pointlightnode(float intensity_input, glm::vec3 color_input);
    virtual ~Pointlightnode() = default;



    void setLightNode(float intensity, glm::vec3 color);

    float getIntensity();
    glm::vec3 getColor();

  private:

    float         lightIntensity_;
    glm::vec3     pointColor_;

};

#endif
