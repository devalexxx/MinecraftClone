//
// Created by Alex on 15/09/2024.
//

#ifndef MCC_CLIENT_UTILS_H
#define MCC_CLIENT_UTILS_H

#include <array>

namespace Mcc
{

	std::array<glm::vec3, 8> vertices
	{{
		{ -1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ -1.0f,-1.0f, -1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, 1.0f },
	}};

	std::array<GLuint, 36> elements {
		1, 3, 7,
		3, 2, 5,
		4, 0, 2,
		6, 2, 0,
		7, 5, 6,
		1, 6, 0,
		1, 4, 3,
		3, 4, 2,
		4, 1, 0,
		6, 5, 2,
		7, 3, 5,
		1, 7, 6
	};

	const char* vertexCode = R"""(
		#version 330

		in vec3 inVertex;
		in vec3 inColor;

		uniform mat4 view;
		uniform mat4 projection;
		uniform mat4 model;

		out vec3 color;

		void main() {
			gl_Position = projection * view * model * vec4(inVertex, 1.0);
			color = inColor;
		}
	)""";

	const char* fragmentCode = R"""(
		#version 330

		in vec3 color;

		out vec4 fragment;

		void main() {
			fragment = vec4(color, 1.0);
		}
	)""";

}

#endif
