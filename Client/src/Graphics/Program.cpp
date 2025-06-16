//
// Created by Alex on 26/08/2024.
//

#include <Client/Graphics/Program.h>
#include <Client/Graphics/Shader.h>
#include <vector>

namespace Mcc
{

	GLuint Program::sUsedProgram = 0;

	Program::Program()
	{
		glCheck(mId = glCreateProgram());
	}

	Program::~Program()
	{
		glCheck(glDeleteProgram(mId));
	}

	void Program::Attach(const Shader& shader) const
	{
		glCheck(glAttachShader(mId, shader.mId));
	}

	void Program::Detach(const Shader& shader) const
	{
		glCheck(glDetachShader(mId, shader.mId));
	}

	void Program::Link() const
	{
		glCheck(glLinkProgram(mId));
		HandleError();
	}

	void Program::Use() const
	{
		if (sUsedProgram != mId)
		{
			glCheck(glUseProgram(mId));
			sUsedProgram = mId;
		}
	}

	GLint Program::GetAttributeLocation(const char* name) const
	{
		GLint location;
		glCheck(location = glGetAttribLocation(mId, name));
		return location;
	}

	GLint Program::GetUniformLocation(const char* name) const
	{
		GLint location;
		glCheck(location = glGetUniformLocation(mId, name));
		return location;
	}

	void Program::SetVertexAttribPointer(GLuint location, GLint size, GLenum type, GLsizei stride, GLsizei offset) const
	{
		Use();
		glCheck(glEnableVertexAttribArray(location));
		glCheck(glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)((intptr_t)offset)));
	}

	void Program::SetVertexAttribPointer(const char* name, GLint size, GLenum type, GLsizei stride, GLsizei offset) const
	{
		SetVertexAttribPointer(GetAttributeLocation(name), size, type, stride, offset);
	}

	void Program::SetVertexAttribPointer(GLuint location, GLint size, GLenum type) const
	{
		SetVertexAttribPointer(location, size, type, 0, 0);
	}

	void Program::SetVertexAttribPointer(const char* name, GLint size, GLenum type) const
	{
		SetVertexAttribPointer(GetAttributeLocation(name), size, type);
	}

	void Program::HandleError() const
	{
		GLint result = GL_FALSE;
		GLint length;
		glCheck(glGetProgramiv(mId, GL_LINK_STATUS, &result));
		if (result == GL_FALSE)
		{
			glCheck(glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &length));
			std::vector<char> message(length);
			glCheck(glGetProgramInfoLog(mId, length, &length, message.data()));
			fmt::print(stderr, "Failed to link program {}:\n\t{}", mId, std::string_view(message.begin(), message.end()));
		}
	}

}