#include "Camera.h"
#include <iostream>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <GameWindow.h>

Camera::Camera() : projectionBindingPoint(1) {
	position = glm::vec3(0, 0.5, 0);
	direction = glm::vec3(0, 0, 1);
	up = glm::vec3(0, 1, 0);

	// Generate buffer
	glGenBuffers(1, &projectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);

	// Initialize data
	float data[16 * 4] = { 0 };
	glBufferData(GL_UNIFORM_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	// Bind to binding point
	glBindBufferBase(GL_UNIFORM_BUFFER, projectionBindingPoint,	projectionBuffer);

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Initialize fov and asp
    fov=55.0;   //  Field of view (angles)
    asp=1;      //  Screen aspect ratio
    updateProjectioneMatrix();
    updateMatrices();
}

Camera::~Camera() {
    // do nothing
}

/**
 *  Bind the uniform block 'Transformations' to a shader.
 * Every shader that wants to use this uniform block MUST call this method.
 * However, this method is called when the shader is created by the renderer. 
 */
void Camera::bindProjectionBuffer(int shader) {
    // Find uniform block id
    int id = glGetUniformBlockIndex(shader, "Tranformations");
    // Set binding point
    if (id > 0) glUniformBlockBinding(shader, id, projectionBindingPoint);
}

/**
 *  This method update the 'Tranformations' buffer with this new model matrix.
 */
void Camera::setModelMatrix(glm::mat4 & modelMatrix) {
    // Bind buffer and copy data
    glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
    // Update model matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 16*sizeof(float), 16*sizeof(float), glm::value_ptr(modelMatrix));
    // Update normal matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 48*sizeof(float), 16*sizeof(float), glm::value_ptr(glm::inverseTranspose(viewMatrix * modelMatrix)));
}

/**
 *  Set the perspective matrix, with the following parameters:
 * fov: field of view in angles.
 * aspRatio: aspectio ratio (width / height).
 * zNear: minimum distance to be rendered
 * zFar: maximum distance to be rendered
 */
void Camera::setPerspective(float fov, float asp, float zNear, float zFar) {
    // Create new projection matrix
    projectionMatrix = glm::perspective<float>(fov, asp, zNear, zFar);

    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
    // Update projection matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16*sizeof(float), glm::value_ptr(projectionMatrix));

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/**
 *  Set the view matrix, with the following parameters:
 * e*: eye position.
 * c*: center.
 * up*: normal vector
 */
void Camera::lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz) {
    // Create new model view matrix
    viewMatrix = glm::lookAt(position, position + direction, glm::vec3(upx, upy, upz));

    // Bind buffer and copy data
    glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 32*sizeof(float), 16*sizeof(float), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 48*sizeof(float), 16*sizeof(float), glm::value_ptr(glm::inverseTranspose(viewMatrix)));
    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::setAspectRatio(float aspectRatio) {
	asp = aspectRatio;
	updateProjectioneMatrix();
}
void Camera::setFieldOfView(float fieldOfView) {
	fov = fieldOfView;
	updateProjectioneMatrix();
}

void Camera::setPosition(float x, float y, float z) {
	glm::vec3 newPosition = glm::vec3(x, y, z);
	glm::vec3 translateVec = newPosition - position;
	translate(translateVec);
}

void Camera::setDirection(float x, float y, float z) {
	// Compute the transformation matrix - use modelMatrix because is point transformation
	glm::mat4 transformation = glm::inverse(vecModelMatrix) * glm::mat4();
	transformation = glm::lookAt(glm::vec3(x,y,z),glm::vec3(0), glm::vec3(0,1,0)) * transformation;
	transformation = vecModelMatrix * transformation;

	// Apply transformation in position
	direction = glm::vec3(transformation * glm::vec4(direction, 1));
	up = glm::vec3(transformation * glm::vec4(up, 1));
	vecModelMatrix = transformation * vecModelMatrix;
	modelMatrix = transformation * modelMatrix;

	updateMatrices();
}

void Camera::setUp(float x, float y, float z) {
//	up = glm::vec3(x, y, z);
	// Update matrices
	updateMatrices();
}

void Camera::updateProjectioneMatrix() {
	// Create new projection matrix
	projectionMatrix = glm::perspective<float>(M_PI*fov/180.0, asp, GameWindow::dim/16.0, GameWindow::dim*16.0);

	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
	// Update projection matrix
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float),
			glm::value_ptr(projectionMatrix));

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::updateMatrices() {
	Transform::updateMatrices();
    // Create new model view matrix
    viewMatrix = glm::lookAt(position, position + direction, up);

    // Bind buffer and copy data
    glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 32*sizeof(float), 16*sizeof(float), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 48*sizeof(float), 16*sizeof(float), glm::value_ptr(glm::inverseTranspose(viewMatrix)));
    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::zoomIn() {
	if (fov > 15)
		fov -= 3;
	updateProjectioneMatrix();
}
void Camera::zoomOut() {
	if (fov < 170)
		fov += 3;
	updateProjectioneMatrix();
}
