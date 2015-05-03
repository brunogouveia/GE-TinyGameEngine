#ifndef CAMERA_H__
#define CAMERA_H__ 

#include "CSCIx239.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Transform.h>

/////////////////////
// Class Camera
// ============
class Camera : public Transform
{
private:
    // Private data
    const int projectionBindingPoint;
    unsigned int projectionBuffer;
    // Matrices
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    // Field of view
    float fov;
    // Aspect ratio
    float asp;

public:
    Camera();
    ~Camera();

    /**
     *  Bind the uniform block 'Transformations' to a shader.
     * Every shader that wants to use this uniform block MUST call this method.
     * However, this method is called when the shader is created by the renderer. 
     */
    void bindProjectionBuffer(int shader);
    
    /**
     *  This method update the 'Tranformations' buffer with this new model matrix.
     */
    void setModelMatrix(glm::mat4 & modelMatrix);

    /**
     *  Set the perspective matrix, with the following parameters:
     * fov: field of view in angles.
     * aspRatio: aspectio ratio (width / height).
     * zNear: minimum distance to be rendered
     * zFar: maximum distance to be rendered
     */
    void setPerspective(float fov, float aspRatio, float zNear, float zFar);

    /**
     *  Set the view matrix, with the following parameters:
     * e*: eye position.
     * c*: center.
     * up*: normal vector
     */
    void lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz);

    void setAspectRatio(float aspectRatio);
    void setFieldOfView(float fieldOfView);
    void setPosition(float x, float y, float z);
    void setDirection(float x, float y, float z);
    void setUp(float x, float y, float z);

    void updateProjectioneMatrix();
    void updateMatrices();

    void zoomIn();
    void zoomOut();
};

#endif
