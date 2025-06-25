//
// Created by Alex on 28/08/2024.
//

#ifndef MCC_CLIENT_GRAPHICS_BUFFER_H
#define MCC_CLIENT_GRAPHICS_BUFFER_H

#include <glad/glad.h>

#include <span>

namespace Mcc
{

	class Buffer
	{
		private:
			static GLuint sBoundBuffer;

		public:
			Buffer(GLenum target);

			~Buffer();

			[[nodiscard]] bool IsValid() const;
			[[nodiscard]] bool HasData() const;

			void Create();
			void Bind() const;

			void SetData(GLsizeiptr size, const void* data, GLenum usage);
			template<typename T>
			void SetData(std::span<T> data, GLenum usage);

		private:
			GLuint mId;
			GLenum mTarget;
			bool   mIsValid;
			bool   mHasData;
	};

}

#include <Client/Graphics/Buffer.inl>

#endif
