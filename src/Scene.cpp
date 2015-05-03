#include "Scene.h"
#include <GameWindow.h>

Scene::Scene() {
	// Initialize camera
	camera = new Camera();
}

Scene::~Scene() {
	// do nothing
}

void Scene::firstDraw() {
	// Set shader
	GBuffer::bindForWriting();
	glUseProgram(GameWindow::getRenderer()->getDeferredShader());
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	for (list<GameObject*>::iterator curObject = objects.begin();
			curObject != objects.end(); ++curObject) {
		(*curObject)->deferredPass();
	}

	ErrCheck("draw 1");
	GBuffer::bindForReading();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// For each object, draw it with regular shader
	for (list<GameObject*>::iterator curObject = objects.begin(); curObject != objects.end(); ++curObject) {
		(*curObject)->rendererPass(false);
	}
	ErrCheck("draw2");
}

void Scene::drawObjectsWithLights() {
	// For each light
	for (list<Light*>::iterator curLight = lights.begin(); curLight != lights.end(); curLight++) {
		// Active current light
		(*curLight)->activeLight();

		// Disable culling
		glDisable(GL_CULL_FACE);
		// Disable blend
		glDisable(GL_BLEND);
		// Set depth func (because at this points, it's probably GL_EQUAL)
		glDepthFunc(GL_LEQUAL);
		// Disable texture
		glDisable(GL_TEXTURE_2D);
		glPolygonOffset(10,0);
		glEnable(GL_POLYGON_OFFSET_FILL);

		// Set depth framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, Light::getShadowFrameBuffer());
		// Set depthShader
		glUseProgram(Light::getShadowShader());

		//  No write to color buffer and no smoothing
	    glShadeModel(GL_FLAT);
	    glColorMask(0,0,0,0);
	    // Set view port to depthTexture dimensions
	    glViewport(0,0,2048,2048);
	    // Clear depth bit
	    glClear(GL_DEPTH_BUFFER_BIT);

	    // Draw objects with current light
		for (list<GameObject*>::iterator curObject = objects.begin(); curObject != objects.end(); ++curObject) {
			(*curObject)->shadowPass();
		}

		glDisable(GL_POLYGON_OFFSET_FILL);

		// Bind default framebuffer back
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	    // Enable texture
	    glEnable(GL_TEXTURE_2D);
	    // Restore normal drawing state
	    glShadeModel(GL_SMOOTH);
	    glColorMask(1,1,1,1);

	    // Set window size to normal
	    glViewport(0,0,GameWindow::width, GameWindow::height);

	    // Enable culling
	    glEnable(GL_CULL_FACE);
	    // Enable blending back
	    glEnable(GL_BLEND);
    	glBlendFunc(GL_ONE, GL_ONE);
    	// Set depth test function
    	glDepthFunc(GL_EQUAL);

		// Draw objects with current light
		for (list<GameObject*>::iterator curObject = objects.begin(); curObject != objects.end(); ++curObject) {
			(*curObject)->rendererPass(true);
		}
	}
}

void Scene::updateScripts() {
	// For each script
	for (list<Script*>::iterator curScript = scripts.begin(); curScript != scripts.end(); curScript++) {
		(*curScript)->OnUpdate();
	}
}

void Scene::addGameObject(GameObject * gameObject) {
	objects.push_back(gameObject);
}

void Scene::addLight(Light * light) {
	lights.push_back(light);
}

void Scene::addScript(Script * script) {
	scripts.push_back(script);
}
