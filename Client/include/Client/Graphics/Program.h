//
// Created by Alex on 26/08/2024.
//

#ifndef MCC_CLIENT_GRAPHICS_PROGRAM_H
#define MCC_CLIENT_GRAPHICS_PROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Mcc
{

	class Shader;

	class Program
	{
		private:
			static GLuint sUsedProgram;

		public:
			Program();
			~Program();
			Program(const Program&)   = delete;
			Program& operator=(const Program&)  = delete;
			Program(Program &&) noexcept;
			Program& operator=(Program&&) noexcept;

			void Attach(const Shader& shader) const;
			void Detach(const Shader& shader) const;
			void Link() const;
			void Use() const;

			GLint GetAttributeLocation(const char* name) const;
			GLint GetUniformLocation(const char* name) const;

			template<glm::length_t R, glm::length_t C, glm::qualifier Q>
			void SetUniformMatrix(GLint location, const glm::mat<R, C, float, Q>& matrix);

			void SetVertexAttribPointer(GLuint location, GLint size, GLenum type, GLsizei stride, GLsizei offset) const;
			void SetVertexAttribPointer(const char* name, GLint size, GLenum type, GLsizei stride, GLsizei offset) const;
			void SetVertexAttribPointer(GLuint location, GLint size, GLenum type) const;
			void SetVertexAttribPointer(const char* name, GLint size, GLenum type) const;

		private:
			void HandleError() const;

		private:
			GLuint mId;
	};

}

#include <Client/Graphics/Program.inl>

#endif
