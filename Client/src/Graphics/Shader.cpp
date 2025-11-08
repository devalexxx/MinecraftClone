// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Graphics/Shader.h"

#include "Client/Graphics/Common.h"

#include "Common/Utils/Logging.h"

#include <fmt/core.h>

#include <vector>

namespace Mcc
{

    Shader::Shader(GLenum kind, const char* code)
    {
        glCheck(mId = glCreateShader(kind));

        glCheck(glShaderSource(mId, 1, &code, nullptr));
        glCheck(glCompileShader(mId));
        HandleError();
    }

    Shader::~Shader()
    {
        glCheck(glDeleteShader(mId));
    }

    void Shader::HandleError() const
    {
        GLint result = GL_FALSE;
        GLint length;
        glCheck(glGetShaderiv(mId, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE)
        {
            glCheck(glGetShaderiv(mId, GL_INFO_LOG_LENGTH, &length));
            std::vector<char> message(length);
            glCheck(glGetShaderInfoLog(mId, length, &length, message.data()));
            MCC_LOG_ERROR("Failed to compile shader {}:\n\t{}", mId, std::string_view(message.begin(), message.end()));
        }
    }

}