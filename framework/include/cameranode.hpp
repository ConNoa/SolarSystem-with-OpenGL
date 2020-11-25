#ifndef CAMERANODE_HPP
#define CAMERANODE_HPP

#include "node.hpp"
#include <glm/glm.hpp>


class Cameranode : public Node {

  public:

    Cameranode();
    ~Cameranode();

    bool          getPerspective();

    bool          getEnabled();
    void          setEnabled(bool is_or_not);

    glm::mat4&    getProjectionMatrix();
    void          setProjectionMatrix(glm::mat4& p_mat_input);

  private:

    bool          isPerspective_;
    bool          isEnabled_;
    glm::mat4     projectionMatrix_;
};

#endif
