#pragma once

#include "Vector4f.h"
#include "Mat4x4f.h"
#include "InputManager.h"

/**
 * @brief Represents a camera in a 3D scene.
 */
class Camera{
	public:
		libMath::vector4f position; /**< The position of the camera. */
		libMath::vector4f rotation; /**< The rotation of the camera. */
		libMath::vector4f orientation; /**< The orientation of the camera. */
		libMath::vector4f lookAt; /**< The point the camera is looking at. */
		libMath::vector4f newlookAt; /**< The point the camera is looking at. */
		libMath::vector4f direction; /**< The direction the camera is facing. */

		float fovy = 0.0f; /**< The vertical field of view angle in radians. */
		float aspectRatio = 0.0f; /**< The aspect ratio of the camera's view. */
		float zNear = 0.0f; /**< The distance to the near clipping plane. */
		float zFar = 0.0f; /**< The distance to the far clipping plane. */

		float prevMouseX = 0.0f; /**< The previous x position of the mouse. */
		float prevMouseY = 0.0f; /**< The previous y position of the mouse. */

		/**
		 * @brief Constructs a Camera object with the specified parameters.
		 * @param position The position of the camera.
		 * @param rotation The rotation of the camera.
		 * @param lookAtp The point the camera is looking at.
		 * @param up The up direction of the camera.
		 * @param fovy The vertical field of view angle in degrees.
		 * @param aspectRatio The aspect ratio of the camera's view.
		 * @param zNear The distance to the near clipping plane.
		 * @param zFar The distance to the far clipping plane.
		 */
		Camera(libMath::vector4f position, libMath::vector4f rotation, libMath::vector4f lookAt, libMath::vector4f orientation, float fovy, float aspectRatio, float zNear, float zFar);

		/**
		 * @brief Calculates the lookAt matrix for the camera.
		 * @return The lookAt matrix.
		 */
		libMath::mat4x4f computeViewMatrix();

		/**
		 * @brief Calculates the perspective matrix for the camera.
		 * @return The perspective matrix.
		 */
		libMath::mat4x4f computeProjectionMatrix();

		/**
		 * @brief Moves the camera based on the specified time step.
		 * @param timeStep The time step for the movement.
		 */
		void move(float timeStep);
};