#ifndef CUBE_H__
#define CUBE_H__ 

#include <CSCIx239.h>
#include <glm/glm.hpp>
#include <GameWindow.h>
#include <GameObject.h>

/////////////////////
// Class Cube
// ==========
class Cube: public GameObject
{
private:
    // Internal data to use opengl
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;
    
    static float vertices[];

public:
    Cube();
    virtual ~Cube();
    
    /**
     *  This method initialize the vao and buffers, so it
     * must be the first method to be called. However it must
     * be called after OpenGL initialization.
     *  You can also choose the basic shader and light shader
     * that will render the GameObject.
     */
    virtual void init(int basicShader, int lightShader);

    /**
     *  This method is called to compute the shadow mapping, 
     * which means that the color is useless. Hence, you shouldn't use
     * any attribute such as textures or normals, just the vertex.
     */
    virtual void shadowPass();

    /**
     *  This method is called when the renderer wants to draw 
     * the cube using color. Hence, attributes such as textures
     * and normals are important here.
     */
    virtual void rendererPass(bool useLight);

    virtual void deferredPass();
};

#endif
