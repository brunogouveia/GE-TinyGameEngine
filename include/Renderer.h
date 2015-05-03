#ifndef RENDERER_H__
#define RENDERER_H__ 

#include "CSCIx239.h"
#include <Scene.h>
#include <Camera.h>
#include <String>

class Renderer
{
private:
    Scene * scene;          //  Scene to be rendered
    Camera * camera;        //  Camera info

    int defaultBasicShader; // Default basic shader
    int defaultLightShader; // Default light shader
    int deferredShader;
    float fov;              //  Field of view (angles)
    float asp;              //  Screen aspect ratio

public:
    Renderer();
    ~Renderer();

    /**
     *  Create a shader from two files, and bind it to all existent uniform blocks.
     * It's really important that you use this method rather than compile shader in your own way.
     */
    int createShaderProg(std::string vertShaderFile, std::string fragShaderFile);
    int getDefaultBasicShader();
    int getDefaultLightShader();
    int getDeferredShader();
    void bindDeferredTextures(int shader);

    void setScene(Scene * s);
    Scene * getScene();
    void setCamera(Camera * c);
    Camera * getCamera();

    // Callback functions
    void display();
    void reshape(int width, int height);
    void special(int key,int x,int y);
    void key(unsigned char ch,int x,int y);
    void idle(); 
};

#endif
