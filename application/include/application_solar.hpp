#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scenegraph.hpp"

#include "node.hpp"
#include "pointlightnode.hpp"
#include "geometrynode.hpp"
#include "cameranode.hpp"
#include "model.hpp"

#define PI 3.1415926


// gpu representation of model
class ApplicationSolar : public Application {
  public:
    // allocate and initialize objects
    ApplicationSolar(std::string const& resource_path);
    // free allocated objects
    ~ApplicationSolar();

    // react to key input
    void keyCallback(int key, int action, int mods);
    //handle delta mouse movement input
    void mouseCallback(double pos_x, double pos_y);
    //handle resizing
    void resizeCallback(unsigned width, unsigned height);
    // draw all objects
    void render() const;
    void renderSolarsystem() const;
    void renderPlanets(Geometrynode* gn) const;
    void renderStars() const;
    void renderOrbits() const;
    void renderSky() const;//fehlt noch//fehlt noch//fehlt noch//fehlt noch//fehlt noch//fehlt noch//fehlt noch

  protected:
    GLuint initTexObj(pixel_data tex_data_in);
    //added  Stars and Orbits
    void initializeStars();
    void initializeOrbits();
    void initializeSkybox();//fehlt noch//fehlt noch//fehlt noch//fehlt noch//fehlt noch//fehlt noch//fehlt noch


    void initializeLabels();

    void initializeTextures(std::vector<std::pair<std::string, pixel_data>> tex_files);
    void initializeFramebuffer();
    void initializePlanets();
    void initializeShaderPrograms();
    void initializeGeometry();
    void initializePointlights();

    void create_cube_map(const char* front, const char* back, const char* top,
                          const char* bottom, const char* left,const char* right,GLuint* tex_cube);

    bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);
    void loadTextures();
    // update uniform values
    void uploadUniforms();
    // upload projection matrix
    void uploadProjection(std::string const& object);
    // upload view matrix
    void uploadView(std::string const& object);



    // cpu representation of model
    model_object planet_object;
    model_object star_object;
    model_object orbit_object;
    model_object sky_object;

    Scenegraph scene;

    // camera transform matrix
    glm::fmat4 view_matrix_;
    // camera projection matrix
    glm::fmat4 projection_matrix_;

    model planet_model;
    model star_model;
    model orbit_model;
    model sky_model;


    int Stars_num;
    GLfloat shader_mode;
    std::vector<Planet>solarsystem_planets_;
    std::vector<float> Stars;
    std::vector<float> m_orbits;
    std::vector<std::string> Labels;
    GLuint framebuffer_tex_obj;
    GLuint rb_handle;
    GLuint fbo_handle;
    std::map<std::string, GLuint>  handles_;


    //DEBUG
    GLuint sky_sphere_tex_obj;
    pixel_data sky_sphere_texture;
    GLuint texcube_obj;

    GLuint texture_object;
    pixel_data test_image;
  //  std::vector<float> Quad_vector= {-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f};

};

#endif
