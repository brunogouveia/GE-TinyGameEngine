#ifndef MATERIAL_H__
#define MATERIAL_H__ 

#include <CSCIx239.h>

class Material
{
private:
	// Static attributes
	static GLuint materialBuffer;
	static int materialBindingPoint;
	static Material defaultMaterial;

	// Material attributes;
	float data[16];
public:
	Material();

	/**
     *  This method initialize the vao and buffers, so it
     * must be the first method to be called. However it must
     * be called after OpenGL initialization.
     */
	static void init();

	/**
     *  Bind the uniform blocks 'Material' to a shader.
     * Every shader that wants to use this uniform block MUST call this method.
     * However, this method is called when the shader is created by the renderer. 
     */
	static void bindMaterialBuffers(int shader);

	/**
	 *	Return the address of default material
	 */
	static Material * getDefaultMaterial();

	/**
     *  This method copy the light data to light buffer.
     * The light buffer is shared by all lights, so this
     * method must be called by each light.
     */
	void activeMaterial();

	// Set methods
	void setEmissive(float r, float g, float b);
	void setEmissive(float color[]);
	void setAmbient(float r, float g, float b);
	void setAmbient(float color[]);
	void setDiffuse(float r, float g, float b);
	void setDiffuse(float color[]);
	void setSpecular(float r, float g, float b);
	void setSpecular(float color[]);
	void setShininess(float shininess);
};

#endif
