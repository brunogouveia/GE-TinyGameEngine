#include "Texture.h"
#include <Util/Image.hpp>

int Texture::defaultTexture = -1;
int Texture::defaultNormalMap = -1;

Texture::Texture(int textureUnit, bool isNormalMap) {
    this->textureUnit = textureUnit;
    glActiveTexture(textureUnit);
    this->texture = (isNormalMap) ? getDefaultNormalMap() : getDefaultTexture();
}

Texture::Texture(int textureUnit, std::string fileName) {
    // Set texture unit
    this->textureUnit = textureUnit;
    glActiveTexture(textureUnit);
    this->texture = LoadTexBMP(fileName.c_str());
}

Texture::~Texture() {
    // Do nothing
}

void Texture::active() const {
    glActiveTexture(textureUnit);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

int Texture::getDefaultTexture() {
    if (defaultTexture == -1) {
        defaultTexture = LoadTexBMP("textures/defaultTexture.bmp");
    }
    return defaultTexture;
}

int Texture::getDefaultNormalMap() {
    if (defaultNormalMap == -1) {
        glActiveTexture(GL_TEXTURE2);
        defaultNormalMap = LoadTexBMP("textures/defaultNormalMap.bmp");
        glActiveTexture(GL_TEXTURE0);
    }
    return defaultNormalMap;   
}
