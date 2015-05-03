#include "GameObject.h"

GameObject::GameObject() : texture(GL_TEXTURE0), normalMap(GL_TEXTURE2, true), material(Material::getDefaultMaterial()) {
    basicShader = -1;
    lightShader = -1;

}

GameObject::~GameObject() {
    // Do nothing
    if (material != Material::getDefaultMaterial()) {
        delete material;
    }
}

/**
 *  This method initialize the vao and buffers, so it
 * must be the first method to be called. However it must
 * be called after OpenGL initialization.
 *  You can also choose the basic shader and light shader
 * that will render the GameObject.
 */
void GameObject::init(int basicShader, int lightShader) {
    this->basicShader = basicShader;
    this->lightShader = lightShader;
}

/**
 *  This method is called to compute the shadow mapping, 
 * which means that the color is useless. Hence, you shouldn't use
 * any attribute such as textures or normals, just the vertex.
 */
void GameObject::shadowPass() {
    // Do nothing
}

/**
 *  This method is called when the renderer wants to draw 
 * the GameObject using color. Hence, attributes such as textures
 * and normals are important here.
 */
void GameObject::rendererPass(bool useLight) {
    // Do nothing
}

void GameObject::deferredPass() {

}

/**
 *  Set a new texture (deffuse texture)
 */
void GameObject::setTexture(Texture newTexture) {
    texture = newTexture;
}

/**
 *  Set a new normal map.
 */
void GameObject::setNormalMap(Texture newNormalMap) {
    normalMap = newNormalMap;
}

/**
 *  Set a new material.
 */
void GameObject::setMaterial(Material * newMaterial) {
    material = newMaterial;
}

//void GameObject::translate(glm::vec3 & t) {
//    glm::mat4 transf = glm::translate(glm::mat4(1.0), t);
//    modelMatrix = transf * modelMatrix;
//}
//
//void GameObject::translate(float tx, float ty, float tz) {
//    glm::vec3 t = glm::vec3(tx, ty, tz);
//    glm::mat4 transf = glm::translate(glm::mat4(1.0), t);
//    modelMatrix = transf * modelMatrix;
//}
//
//void GameObject::rotate(float angle, glm::vec3 & up) {
//    glm::mat4 transf = glm::rotate(glm::mat4(1.0), angle, up);
//    modelMatrix = transf * modelMatrix;
//}
//
//void GameObject::scale(glm::vec3 & s) {
//    glm::mat4 transf = glm::scale(glm::mat4(1.0), s);
//    modelMatrix = transf * modelMatrix;
//}
//
//void GameObject::scale(float sx, float sy, float sz) {
//    glm::vec3 s = glm::vec3(sx, sy, sz);
//    glm::mat4 transf = glm::scale(glm::mat4(1.0), s);
//    modelMatrix = transf * modelMatrix;
//}
