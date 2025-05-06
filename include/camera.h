#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
#include <corecrt_math_defines.h>
#include <glm/ext/matrix_transform.hpp>

struct SCamera
{
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	const float MovementSpeed = .5f;
	float MouseSensitivity = 1.f;

	glm::mat4 GetViewMatrix() const {
		return glm::lookAt(Position, Position + Front, Up);
	}

};


void InitCamera(SCamera& in)
{
	in.Front = glm::vec3(0.0f, 0.0f, -1.0f);
	in.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	in.Up = glm::vec3(0.0f, 1.0f, 0.0f);
	in.WorldUp = in.Up;
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));

	in.Yaw = -90.f;
	in.Pitch = 0.f;
}

float cam_dist = 2.f;

void MoveAndOrientCamera(SCamera& in, glm::vec3 target, float distance, float xoffset, float yoffset)
{
	in.Yaw -= xoffset;
	in.Pitch -= yoffset;

	if (in.Pitch > 89.0f) 
		in.Pitch = 89.0f;

	if (in.Pitch < -89.0f) 
		in.Pitch = -89.0f;
	float yaw = (M_PI / 180.f) * in.Yaw;
	float pitch = (M_PI / 180.f) * in.Pitch;
	float px = cos(yaw) * cos(pitch) * distance;
	float py = sin(pitch) * distance;
	float pz = sin(yaw) * cos(pitch) * distance;
	glm::vec3 offset = glm::vec3(px, py, pz);

	in.Position = target + offset;

	in.Front = glm::normalize(target - in.Position);
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
	in.Up = glm::normalize(glm::cross(in.Right, in.Front));

}