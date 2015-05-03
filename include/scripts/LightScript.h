#ifndef CUBESCRIPT_H__
#define CUBESCRIPT_H__ 

#include <Script.h>

class LightScript : public Script
{
public:	
	/**
	 * 	This method is called before every frame.
	 */
	virtual void OnUpdate();
};

#endif