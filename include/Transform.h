#ifndef TRANSFORM_H__
#define TRANSFORM_H__ 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Transform {
protected:
	glm::mat4 modelMatrix;
	glm::mat4 vecModelMatrix;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

public:
	Transform();

	/**
	 * Virtual destructor
	 */
	virtual ~Transform();

	virtual void updateMatrices();

	/**
	 * 	Translate by a vector 't'.
	 */
	virtual void translate(glm::vec3 & t);
	virtual void translate(float tx, float ty, float tz);

	/**
	 * 	Translate by a vector 't'.
	 */
	virtual void localTranslate(glm::vec3 & t);
	virtual void localTranslate(float tx, float ty, float tz);

	/**
	 *  Rotate the transform by 'rad' radians around the vector normal.
	 */
	virtual void rotate(float rad, glm::vec3 & normal);

	/**
	 *  Rotate the transform by 'rad' radians around the vector normal.
	 */
	virtual void localRotate(float rad, glm::vec3 & normal);

	/**
	 *  Scale the transform by a vector 's'.
	 */
	virtual void scale(glm::vec3 & s);
	virtual void scale(float sx, float sy, float sz);

	/**
	 *  Scale the transform by a vector 's'.
	 */
	virtual void localScale(glm::vec3 & s);
	virtual void localScale(float sx, float sy, float sz);

	virtual void moveUp(float distance);
	virtual void moveDown(float distance);
	virtual void moveForward(float distance);
	virtual void moveBackward(float distance);
	virtual void moveRight(float distance);
	virtual void moveLeft(float distance);

	virtual glm::vec3 toTransform(glm::vec3 v);
	virtual glm::vec3 fromTransform(glm::vec3 v);
	virtual glm::vec3 toTransformVec(glm::vec3 v);
	virtual glm::vec3 fromTransformVec(glm::vec3 v);

};

#endif
