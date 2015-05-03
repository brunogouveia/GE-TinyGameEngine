#ifndef SCENE_H__
#define SCENE_H__ 

#include <List>
#include <GameObject.h>
#include <Light.h>
#include <Camera.h>
#include <Script.h>
#include <GBuffer.h>

using namespace std;

class Scene
{
public:
	Camera * camera;
	// List of objects
	list<GameObject*> objects;
	// List of lights
	list<Light*> lights;
	// List of scripts
	list<Script*> scripts;

public:
	Scene();
	~Scene();

	/**
	 *	This method is the first step of the renderer process.
	 * It renderer all objects with their basic shaders
	 */
	void firstDraw();

	/**
	 *	This method is the second step of the renderer process.
	 * it renderer all objects, firstly with the shadow shader
	 * (in order to compute the shadow mapping) and secondly rendering
	 * with their light shaders.
	 */
	void drawObjectsWithLights();

	/**
	 *	Call the OnUpdate method for all scripts
	 */
	void updateScripts();

	/**
	 *	Add a new gameobject to scene
	 */
	void addGameObject(GameObject * gameObject);

	/**
	 *	Add a new light to scene
	 */
	void addLight(Light * light);

	/**
	 *	Add a new script to scene
	 */
	void addScript(Script * script);
};

#endif
