#ifndef MESHOBJECT_h__
#define MESHOBJECT_h__ 

#include <GameObject.h>
#include <String>
#include <vector>

class Shape {
public:
	GLsizei begin;
	GLsizei end;
	Texture texture;
	Texture normalMap;
	Material * material;

	Shape(int b, int e) :
		begin(b),
		end(e),
		texture(GL_TEXTURE0),
		normalMap(GL_TEXTURE1, true),
		material(NULL)
	{};
};

class MeshObject : public GameObject
{
private:
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;

    // Vector of shapes
   std::vector<Shape *> shapeVector;


    int numVertices;

public:
    MeshObject(glm::mat4 modelMatrix);
    virtual ~MeshObject();

    /**
     *  This method initialize the vao and buffers, so it
     * must be the first method to be called. However it must
     * be called after OpenGL initialization.
     *  You can also choose the basic shader and light shader
     * that will render the MeshObject.
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

    /**
     *  This method loads the triangle mesh from a file. If you don't
     * call this methods, this mesh is gonna be useless.
     */
    void loadFromFile(char * fileName);
private:
};

#endif
