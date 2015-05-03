#ifndef GAMEOBJECT_H__
#define GAMEOBJECT_H__ 

#include <CSCIx239.h>
#include <List>
#include <Texture.h>
#include <Material.h>
#include <Transform.h>

class GameObject : public Transform
{
protected:
    int basicShader;        // Shader to render this object
    int lightShader;        // Shader that render the light effect
    Texture texture;        // Texture
    Texture normalMap;      // Normal map
    Material * material;    // Material

public:
    GameObject();
    virtual ~GameObject();
    
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
     * the GameObject using color. Hence, attributes such as textures
     * and normals are important here.
     */
    virtual void rendererPass(bool useLight);

    virtual void deferredPass();

    /**
     *  Set a new texture (deffuse texture)
     */
    virtual void setTexture(Texture newTexture);

    /**
     *  Set a new normal map.
     */
    virtual void setNormalMap(Texture newNormalMap);

    /**
     *  Set a new material.
     */
    virtual void setMaterial(Material * newMaterial);

//    // Transform functions
//    virtual void translate(glm::vec3 & t);
//    virtual void translate(float tx, float ty, float tz);
//    virtual void rotate(float angle, glm::vec3 & up);
//    virtual void scale(glm::vec3 & s);
//    virtual void scale(float sx, float sy, float sz);
};

#endif
