#include "Camera.h"

Camera::Camera(libMath::vector4f position, libMath::vector4f rotation, libMath::vector4f lookAt, libMath::vector4f orientation, float fovy, float aspectRatio, float zNear, float zFar) {
    this->orientation = libMath::normalize(orientation);
    this->rotation = rotation;
    this->position = position;
    this->lookAt = lookAt;

    this->fovy = fovy;
    this->aspectRatio = aspectRatio;
    this->zNear = zNear;
    this->zFar = zFar;
}

libMath::mat4x4f Camera::computeViewMatrix() {
    libMath::mat4x4f m = libMath::make_identityf();
	libMath::vector4f up, forward, right;

	forward = libMath::normalize(newlookAt - position);
	forward.w = 0;
	
    right = libMath::normalize(libMath::cross(forward, (orientation)));
	right.w = 0;
	
    up = libMath::cross(right, forward);
	up.w = 0;

	m.data2D[0][0] = right.x;
	m.data2D[1][0] = right.y;
	m.data2D[2][0] = right.z;

	m.data2D[0][1] = up.x;
	m.data2D[1][1] = up.y;
	m.data2D[2][1] = up.z;
	
    m.data2D[0][2] = -forward.x;
	m.data2D[1][2] = -forward.y;
	m.data2D[2][2] = -forward.z;
	
    m.data2D[3][0] = -(libMath::dot(right, position));
	m.data2D[3][1] = -(libMath::dot(up, position));
	m.data2D[3][2] = libMath::dot(forward, position);

    return libMath::transpose(m);
}

libMath::mat4x4f Camera::computeProjectionMatrix() {
    libMath::mat4x4f projMatrix;

    projMatrix.rows[0] = { (1.0f / (aspectRatio * tan(fovy * 0.5f))), 0, 0, 0 };
    projMatrix.rows[1] = { 0, (1.0f / tan(fovy * 0.5f)), 0, 0 };
    projMatrix.rows[2] = { 0, 0, -((zFar + zNear) / (zFar - zNear)), ((2.0f * zFar * zNear) / (zFar - zNear)) };
    projMatrix.rows[3] = { 0, 0, -1, 1 };

    return projMatrix;
}

void Camera::move(float timeStep) {
    float speed = 10.0f;
    float rotationSpeedX = InputManager::mouseState.posX - this->prevMouseX;
    float rotationSpeedY = InputManager::mouseState.posY - this->prevMouseY;    

    if (rotationSpeedX != 0 || rotationSpeedY != 0) {
        rotation.y += rotationSpeedX * 0.5f;
        rotation.x += rotationSpeedY * 0.5f;

        libMath::mat4x4f rotationMatrix = libMath::make_rotate(libMath::degreesToRadians(rotation.x), libMath::degreesToRadians(rotation.y), 0.0f);
        newlookAt = libMath::make_vector4f(0, 0, -1, 1);

        newlookAt = rotationMatrix * newlookAt;
        // std::cout << libMath::toString(newlookAt) << std::endl;
    }

    this->direction = libMath::normalize(newlookAt - position);

    if (InputManager::keysState[GLFW_KEY_UP]) {
        position = position + (direction * timeStep * speed);
        newlookAt = newlookAt + (direction * timeStep * speed);
    }

    if (InputManager::keysState[GLFW_KEY_DOWN]) {
        position = position - (direction * timeStep * speed);
        newlookAt = newlookAt - (direction * timeStep * speed);
    }

    if (InputManager::keysState[GLFW_KEY_LEFT]) {
        libMath::vector4f right = libMath::cross(direction, libMath::make_vector4f(0, 1, 0, 0));
        position = position - (right * timeStep * speed);
        newlookAt = newlookAt - (right * timeStep * speed);
    }

    if (InputManager::keysState[GLFW_KEY_RIGHT]) {
        libMath::vector4f right = libMath::cross(direction, libMath::make_vector4f(0, 1, 0, 0));
        position = position + (right * timeStep * speed);
        newlookAt = newlookAt + (right * timeStep * speed);
    }

    this->prevMouseX = InputManager::mouseState.posX;
    this->prevMouseY = InputManager::mouseState.posY;
}
