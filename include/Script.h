#ifndef SCRIPT_H__
#define SCRIPT_H__ 

#include <glm/glm.hpp>
#include <Transform.h>
#include <String>
#include <stdio.h>

class Script
{
protected:
	// Transform Reference
	Transform * transform;

public:
	virtual ~Script(){};
	/**
	 *  This if the first method called. You should initialize things here.
	 */
	virtual void OnCreate();
	
	/**
	 * 	This method is called before every frame.
	 */
	virtual void OnUpdate();

	/**
	 *  This if the last method called. You should clean things here.
	 */
	virtual void OnDestroy();

	/**
	 *	This method creates a new script and bind it with the transform.
	 */
	static Script * GetScriptByName(Transform * transform, std::string scriptName);
};

#endif
