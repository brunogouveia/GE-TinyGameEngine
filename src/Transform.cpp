/*
 * Transform.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: Tiago
 */
#include "Transform.h"

Transform::Transform() {
	position = glm::vec3(0);
	direction = glm::vec3(0, 0, 1);
	up = glm::vec3(0, 1, 0);
}

/**
 * Virtual destructor
 */
Transform::~Transform() {
}

void Transform::updateMatrices() {
//	vecModelMatrix = glm::mat4(glm::vec4(glm::cross(up, direction), 0),
//			glm::vec4(up, 0), glm::vec4(direction, 0), glm::vec4(0, 0, 0, 1));
//	modelMatrix = glm::translate(glm::mat4(), position) * vecModelMatrix;
}

/**
 * 	Translate by a vector 't'.
 */
void Transform::translate(glm::vec3 & t) {
	// Update position
	position += t;
	// Update modelMatrix
	modelMatrix = glm::translate(glm::mat4(), t) * modelMatrix;
	// Call update matrices
	updateMatrices();
}
void Transform::translate(float tx, float ty, float tz) {
	glm::vec3 t= glm::vec3(tx, ty, tz);
	translate(t);
}

/**
 * 	Translate by a vector 't'.
 */
void Transform::localTranslate(glm::vec3 & t) {
	// Compute the transformation matrix - use modelMatrix because is point transformation
	glm::mat4 transformation = glm::inverse(modelMatrix) * glm::mat4();
	transformation = glm::translate(glm::mat4(), t) * transformation;
	transformation = modelMatrix * transformation;

	// Apply transformation in position
	position = glm::vec3(transformation * glm::vec4(position, 1));
	modelMatrix = transformation * modelMatrix;

	// Update matrices
	updateMatrices();
}

void Transform::localTranslate(float tx, float ty, float tz) {
	// Compute the transformation matrix - use modelMatrix because is point transformation
	glm::mat4 transformation = glm::inverse(modelMatrix) * glm::mat4();
	transformation = glm::translate(glm::mat4(), glm::vec3(tx, ty, tz))
			* transformation;
	transformation = modelMatrix * transformation;

	// Apply transformation in position
	position = glm::vec3(transformation * glm::vec4(position, 1));
	modelMatrix = transformation * modelMatrix;

	// Update matrices
	updateMatrices();
}

/**
 *  Rotate the transform by 'rad' radians around the vector normal.
 */
void Transform::rotate(float rad, glm::vec3 & normal) {
	// Rotate point, direction and up
	position = glm::rotate(position, rad, normal);
	direction = glm::rotate(direction, rad, normal);
	up = glm::rotate(up, rad, normal);

	vecModelMatrix = glm::rotate(glm::mat4(), rad, normal) * vecModelMatrix;
	modelMatrix = glm::rotate(glm::mat4(), rad, normal) * modelMatrix;
	// Update matrices
	updateMatrices();
}

/**
 *  Rotate the transform by 'rad' radians around the vector normal.
 */
void Transform::localRotate(float rad, glm::vec3 & normal) {
	// Compute the transformation matrix - use vecModelMatrix because is vector transformation
	glm::mat4 transformation = glm::inverse(vecModelMatrix) * glm::mat4();
	transformation = glm::rotate(glm::mat4(), rad, normal) * transformation;
	transformation = vecModelMatrix * transformation;

	// Apply transformation in direction and up vectors
	direction = glm::vec3(transformation * glm::vec4(direction, 1));
	up = glm::vec3(transformation * glm::vec4(up, 1));

	vecModelMatrix = transformation * vecModelMatrix;
	modelMatrix = transformation * modelMatrix;

	// Update matrices
	updateMatrices();
}

/**
 *  Scale the transform by a vector 's'.
 */
void Transform::scale(glm::vec3 & s) {
	glm::mat4 transf = glm::scale(glm::mat4(1.0), s);
	modelMatrix = transf * modelMatrix;
	// Scale point, direction and up
	position = position * s;

	// Update matrices
	updateMatrices();
}
void Transform::scale(float sx, float sy, float sz) {
	// Scale point, direction and up
	glm::vec3 s = glm::vec3(sx, sy, sz);
	scale(s);
}

/**
 *  Scale the transform by a vector 's' in the model coordinate system.
 */
void Transform::localScale(glm::vec3 & s) {
	// Compute the transformation matrix - use odelMatrix
	glm::mat4 transformation = glm::inverse(modelMatrix) * glm::mat4();
	transformation = glm::scale(glm::mat4(), s) * transformation;
	transformation = modelMatrix * transformation;

	// Apply transformation in position
	position = glm::vec3(transformation * glm::vec4(position, 1));

	// Compute the transformation matrix - use vecModelMatrix because is vector transformation
	transformation = glm::inverse(vecModelMatrix) * glm::mat4();
	transformation = glm::scale(glm::mat4(), s) * transformation;
	transformation = vecModelMatrix * transformation;

	// Apply transformation in direction and up vectors
	direction = glm::vec3(transformation * glm::vec4(direction, 1));
	up = glm::vec3(transformation * glm::vec4(up, 1));

	// Update matrices
	updateMatrices();
}
void Transform::localScale(float sx, float sy, float sz) {
	glm::vec3 s = glm::vec3(sx, sy, sz);
	localScale(s);
}

void Transform::moveUp(float distance) {
//	// Set new position
//	position += distance * up;
//	// Update matrices
//	updateMatrices();
	glm::vec3 t = distance * up;
	translate(t);
}

void Transform::moveDown(float distance) {
//	// Set new position
//	position -= distance * up;
//	// Update matrices
//	updateMatrices();
	glm::vec3 t = -distance * up;
	translate(t);
}

void Transform::moveForward(float distance) {
//	// Set new position
//	position += distance * direction;
//	// Update matrices
//	updateMatrices();
	glm::vec3 t = distance * direction;
	translate(t);
}

void Transform::moveBackward(float distance) {
//	// Set new position
//	position -= distance * direction;
//	// Update matrices
//	updateMatrices();
	glm::vec3 t = -distance * direction;
	translate(t);
}

void Transform::moveLeft(float distance) {
//	// Set new position
//	position += distance * glm::cross(up, direction);
//	// Update matrices
//	updateMatrices();
	glm::vec3 t = distance * glm::cross(up, direction);
	translate(t);
}

void Transform::moveRight(float distance) {
//	// Set new position
//	position += distance * -glm::cross(up, direction);
//	// Update matrices
//	updateMatrices();
	glm::vec3 t = -distance * glm::cross(up, direction);
	translate(t);
}
glm::vec3 Transform::toTransform(glm::vec3 v) {
	return glm::vec3(glm::inverse(modelMatrix) * glm::vec4(v, 0));
}
glm::vec3 Transform::fromTransform(glm::vec3 v) {
	return glm::vec3(modelMatrix * glm::vec4(v, 0));
}
glm::vec3 Transform::toTransformVec(glm::vec3 v) {
	return glm::vec3(glm::inverse(vecModelMatrix) * glm::vec4(v, 0));
}
glm::vec3 Transform::fromTransformVec(glm::vec3 v) {
	return glm::vec3(vecModelMatrix * glm::vec4(v, 0));
}

