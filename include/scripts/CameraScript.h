#ifndef CAMERA_SCRIPT_H__
#define CAMERA_SCRIPT_H__

#include <Script.h>
#include <Camera.h>

class CameraScript: public Script {
private:
	Camera * camera;
public:
	/**
	 * 	This method is called before every frame.
	 */
	virtual void OnUpdate();

};

#endif
