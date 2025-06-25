//
// Created by Alex on 29/08/2024.
//

#ifndef MCC_CLIENT_GRAPHICS_VERTEXARRAY_H
#define MCC_CLIENT_GRAPHICS_VERTEXARRAY_H

#include "glad/glad.h"

namespace Mcc
{

	class VertexArray
	{
		public:
			static bool IsThereAnyBound();

		private:
			static GLuint sBoundVertexArray;

		public:
			VertexArray();

			~VertexArray();

			[[nodiscard]] bool IsValid() const;

			void Create();
			void Bind() const;

		private:
			GLuint mId;
			bool   mIsValid;
	};

}

#endif
