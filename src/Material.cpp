#include "Material.h"
#include <cstring>

// Static attributes
GLuint   Material::materialBuffer;
int      Material::materialBindingPoint = 2;
Material Material::defaultMaterial;

Material::Material() {
    // do nothing
    float newData[16] = {
        // All attributes should be treated as a vec3 with last component equal 1.0 (except Specular)
        // Emissive
        0.0, 0.0, 0.0, 1.0,
        // Ambient
        1.0, 1.0, 1.0, 1.0,
        // Diffuse
        1.0, 1.0, 1.0, 1.0,
        // Specular - shininess (shininess is the last float of Specular)
        1.0, 1.0, 1.0, 100.0
    };

    // Copy to light's data
    std::memcpy(data, newData, sizeof(data));
}

void Material::init() {
    // Create and buffer
    glGenBuffers(1, &materialBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);

    // Initialize buffer
    float newData[16] = {
        // All attributes should be treated as a vec3 with last component equal 1.0 (except Specular)
        // Emissive
        1.0, 1.0, 1.0, 1.0,
        // Ambient
        1.0, 1.0, 1.0, 1.0,
        // Diffuse
        1.0, 1.0, 1.0, 1.0,
        // Specular - shininess (shininess is the last float of Specular)
        1.0, 1.0, 1.0, 100.0
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(newData), newData, GL_DYNAMIC_DRAW);

    // Set binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingPoint, materialBuffer);

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Material::bindMaterialBuffers(int shader) {
    // Find global light block index
    int id = glGetUniformBlockIndex(shader, "Material");
    // Bind if block exists
    if (id >= 0)  {
        glUniformBlockBinding(shader, id, materialBindingPoint);
    }
}

Material * Material::getDefaultMaterial() {
    return &defaultMaterial;
}

void Material::activeMaterial() {
    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);

    // Copy material's data to buffer
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), data);

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Material::setEmissive(float r, float g, float b) {
    // Copy to data
    data[0] = r; data[1] = g; data[2] = b;
}

void Material::setEmissive(float color[]) {
	setEmissive(color[0], color[1], color[2]);
}

void Material::setAmbient(float r, float g, float b) {
    // Copy to data
    data[4] = r; data[5] = g; data[6] = b;
}

void Material::setAmbient(float color[]) {
	setAmbient(color[0], color[1], color[2]);
}

void Material::setDiffuse(float r, float g, float b) {
    // Copy to data
    data[8] = r; data[9] = g; data[10] = b;
}

void Material::setDiffuse(float color[]) {
	setDiffuse(color[0], color[1], color[2]);
}

void Material::setSpecular(float r, float g, float b) {
    // Copy to data
    data[12] = r; data[13] = g; data[14] = b;
}

void Material::setSpecular(float color[]) {
	setSpecular(color[0], color[1], color[2]);
}
void Material::setShininess(float shininess) {
    // Copy to data
    data[15] = shininess;
}
