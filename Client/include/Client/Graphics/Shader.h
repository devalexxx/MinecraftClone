//
// Created by Alex on 26/08/2024.
//

#ifndef MINECRAFT_SHADER_H
#define MINECRAFT_SHADER_H

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

		private:
			GLuint mId;

			friend class Program;
	};

}

#endif
