#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include "node.hpp"
#include <glm/glm.hpp>


class Pointlightnode : public Node {

  public:

    Pointlightnode();
    ~Pointlightnode();

    bool          getPerspective();

    bool          getEnabled();
    void          setEnabled(bool is_or_not);

    glm::mat4&    getProjectionMatrix();
    void          setProjectionMatrix(glm::mat4& p_mat_input);

  private:

    glm::vec3     pointColor_;
    float         lightIntensity_;

    bool          isPerspective_;
    bool          isEnabled_;
    glm::mat4     projectionMatrix_;
};

#endif
