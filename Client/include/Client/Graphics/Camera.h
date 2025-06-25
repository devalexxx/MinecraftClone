//
// Created by Alex on 23/08/2024.
//

#ifndef MCC_CLIENT_GRAPHICS_CAMERA_H
#define MCC_CLIENT_GRAPHICS_CAMERA_H

#include "Common/Entity/Player.h"

#include <glm/glm.hpp>

namespace Mcc
{

	class Camera
	{
		public:
			explicit Camera(Player& player);

			~Camera();

			void Translate(const glm::vec3& position);
			void Rotate(float angle, const glm::vec3& axe);
			void Scale(const glm::vec3& scale);

			void LookAt(const glm::vec3& target);

			void SetFov(float fov);
			void SetAspectRatio(float ratio);
			void SetNear(float near);
			void SetFar(float far);

			[[nodiscard]] const glm::mat4& GetMatrix() const;
			[[nodiscard]] glm::quat        GetPlayerRotation() const;

		private:
			void UpdateMatrix();

		private:
			Player&           mPlayer;
			Player::HandlerId mETUHandlerId;

			float mFov;
			float mScreenRatio;
			float mNear;
			float mFar;

			glm::mat4 mMatrix;
	};

}

#endif
