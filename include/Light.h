#ifndef LIGHT_H__
#define LIGHT_H__ 

#include <CSCIx239.h>
#include <Transform.h>

class Light: public Transform
{
private:
    // Static attributes, this is global to all lights
    static GLuint globalLightBuffer;
    static GLuint lightBuffer; // This is shared between all lights.
    static int globalLightBindingPoint;
    static int lightBindingPoint;

    // Shadow
    static GLuint shadowBuffer;
    static int shadowBindingPoint;
    static GLuint shadowFrameBuffer;
    static int shadowShader;
    static GLuint shadowTexture;


    // Light attributes
    float angle;
    float data[12];

public:
    // Shadow texture planes
    static double Svec[4];   // Texture planes S
    static double Tvec[4];   // Texture planes T
    static double Rvec[4];   // Texture planes R
    static double Qvec[4];   // Texture planes Q
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Light();
    ~Light();

    // Set methods
    void setLightAngle(float angle);
    void setPosition(float x, float y, float z);
    void setPosition(float position[]);
	void setDirection(float x, float y, float z);
	void setDirection(float direction[]);
    void setDiffuse(float r, float g, float b);
    void setDiffuse(float diffuse[]);
    void setSpecular(float r, float g, float b);
    void setSpecular(float specular[]);

    /**
     *  This method copy the light data to light buffer.
     * The light buffer is shared by all lights, so this
     * method must be called by each light.
     */
    void activeLight();

    /**
     *  This method copy the light matrices to shados buffer.
     * The shadow buffer is shared by all lights, so this
     * method must be called by each light.
     *  These matrices are used in the shadow mapping technique.
     */
    void updateMatrices();

    /**
     *  This method initialize the vao and buffers, so it
     * must be the first method to be called. However it must
     * be called after OpenGL initialization.
     */
    static void init();

    /**
     *  This method initialize the vao and buffers, so it
     * must be the second method to be called. However it must
     * be called after Renderer initialization (because it compiles
     * a shader and need the Renderer for that).
     */
    static void initShadowMap(int shadowShader);

    /**
     *  Bind the uniform blocks 'GlobalLight', 'Light' and 'Shadows' to a shader.
     * Every shader that wants to use this uniform block MUST call this method.
     * However, this method is called when the shader is created by the renderer. 
     */
    static void bindLightBuffers(int shader);

    // Get methods
    static GLuint getShadowFrameBuffer();
    static int getShadowShader();
    static GLuint getShadowTexture();

    /**
     *  Translate by a vector 't'.
     */
    virtual void translate(glm::vec3 & t);
    virtual void translate(float tx, float ty, float tz);

    /**
     *  Rotate the transform by 'rad' radians around the vector normal.
     */
    virtual void rotate(float rad, glm::vec3 & normal);

    /**
     *  Scale the transform by a vector 's'.
     */
    virtual void scale(glm::vec3 & s);
    virtual void scale(float sx, float sy, float sz);
    
};

#endif
