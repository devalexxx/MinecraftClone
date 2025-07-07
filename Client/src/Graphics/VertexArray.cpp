//
// Created by Alex on 29/08/2024.
//

#include "Client/Graphics/VertexArray.h"
#include "Client/Graphics/Common.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	GLuint VertexArray::sBoundVertexArray = 0;

	bool VertexArray::IsThereAnyBound()
	{
		return sBoundVertexArray != 0;
	}

	VertexArray::VertexArray() :
		mId(0)
	{}

	VertexArray::~VertexArray()
	{
		if (IsValid())
		{
			glCheck(glDeleteVertexArrays(1, &mId));
		}
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept :
		mId(other.mId)
	{
		other.mId = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		if (this != &other)
		{
			if (IsValid())
			{
				this->~VertexArray();
			}

			mId		  = other.mId;
			other.mId = 0;
		}
		return *this;
	}

	bool VertexArray::IsValid() const
	{
		bool result;
		glCheck(result = glIsVertexArray(mId) == GL_TRUE);
		return result;
	}

	void VertexArray::Create()
	{
		glCheck(glGenVertexArrays(1, &mId));
		glBindVertexArray(mId);
		MCC_ASSERT(IsValid(), "VertexArray creation failed");
	}

	void VertexArray::Bind() const
	{
		MCC_ASSERT(IsValid(), "A VertexArray must be valid (eg. created) to be bound");
		if (sBoundVertexArray != mId)
		{
			glCheck(glBindVertexArray(mId));
			sBoundVertexArray = mId;
		}
	}

}