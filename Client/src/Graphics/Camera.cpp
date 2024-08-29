//
// Created by Alex on 23/08/2024.
//

#include <Client/Graphics/Camera.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Mcc
{
	Camera::Camera(Player& player) :
		mPlayer(player),
		mFov(75.f),
		mScreenRatio(4.f / 3.f),
		mNear(0.1f),
		mFar(100.f),
		mMatrix(glm::perspective(mFov, mScreenRatio, mNear, mFar) * mPlayer.GetTransform())
	{
		mETUHandlerId = player.Subscribe<EntityTransformUpdateEvent>([this](const EntityTransformUpdateEvent&) {
			UpdateMatrix();
		});
	}

	Camera::~Camera()
	{
		mPlayer.Withdraw(mETUHandlerId);
	}

	void Camera::Translate(const glm::vec3& position)
	{
		mPlayer.Translate(position);
	}

	void Camera::Rotate(float angle, const glm::vec3& axe)
	{
		mPlayer.Rotate(angle, axe);
	}

	void Camera::Scale(const glm::vec3& scale)
	{
		mPlayer.Scale(scale);
	}

	void Camera::LookAt(const glm::vec3& target)
	{
		mPlayer.LookAt(target);
	}

	const glm::mat4& Camera::GetMatrix() const
	{
		return mMatrix;
	}

	glm::quat Camera::GetPlayerRotation() const
	{
		return mPlayer.GetRotation();
	}

	void Camera::UpdateMatrix()
	{
		mMatrix = glm::perspective(glm::radians(mFov), mScreenRatio, mNear, mFar) * mPlayer.GetHeadTransform();
	}

	void Camera::SetFov(float fov)
	{
		mFov = fov;
		UpdateMatrix();
	}

	void Camera::SetAspectRatio(float ratio)
	{
		mScreenRatio = ratio;
		UpdateMatrix();
	}

	void Camera::SetNear(float near)
	{
		mNear = near;
		UpdateMatrix();
	}

	void Camera::SetFar(float far)
	{
		mFar = far;
		UpdateMatrix();
	}

}