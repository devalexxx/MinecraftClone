//
// Created by Alex on 29/08/2024.
//

#include "Client/Graphics/VertexArray.h"
#include "Client/Graphics/Common.h"

#include <cassert>

namespace Mcc
{

	GLuint VertexArray::sBoundVertexArray = 0;

	bool VertexArray::IsThereAnyBound()
	{
		return sBoundVertexArray != 0;
	}

	VertexArray::VertexArray() :
		mId(0), mIsValid(false)
	{}

	VertexArray::~VertexArray()
	{
		if (mIsValid)
			glCheck(glDeleteVertexArrays(1, &mId));
	}

	bool VertexArray::IsValid() const
	{
		return mIsValid;
	}

	void VertexArray::Create()
	{
		glCheck(glGenVertexArrays(1, &mId));
		if (mId != 0)
			mIsValid = true;
	}

	void VertexArray::Bind() const
	{
		assert(mIsValid);
		if (sBoundVertexArray != mId)
			glCheck(glBindVertexArray(mId));
		sBoundVertexArray = mId;
	}

}