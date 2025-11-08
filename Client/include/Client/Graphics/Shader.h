// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_GRAPHICS_SHADER_H
#define MCC_CLIENT_GRAPHICS_SHADER_H

#include <glad/glad.h>

namespace Mcc
{

    class Shader
    {
      public:
        Shader(GLenum kind, const char* code);
        ~Shader();

      private:
        void HandleError() const;

        GLuint mId;

        friend class Program;
    };

}

#endif
