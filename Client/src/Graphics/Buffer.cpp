//
// Created by Alex on 28/08/2024.
//

#include "Client/Graphics/Buffer.h"
#include "Client/Graphics/VertexArray.h"

#include "Common/Utils/Assert.h"

namespace Mcc
{

	GLuint Buffer::sBoundBuffer = 0;

	Buffer::Buffer(GLenum target) :
		mId(0), mTarget(target), mIsValid(false), mHasData(false)
	{}

	Buffer::~Buffer()
	{
		if (mIsValid)
			glCheck(glDeleteBuffers(1, &mId));
	}

	bool Buffer::IsValid() const
	{
		return mIsValid;
	}

	bool Buffer::HasData() const
	{
		return mHasData;
	}

	void Buffer::Create()
	{
		MCC_ASSERT(VertexArray::IsThereAnyBound(), "A VertexArray must be bound to create a Buffer");
		glCheck(glGenBuffers(1, &mId));
		if (mId != 0)
			mIsValid = true;
	}

	void Buffer::Bind() const
	{
		MCC_ASSERT(mIsValid, "A Buffer must be valid (eg. created) to be bound");
		if (sBoundBuffer != mId)
		{
			glCheck(glBindBuffer(mTarget, mId));
			sBoundBuffer = mId;
		}
	}

	void Buffer::SetData(GLsizeiptr size, const void* data, GLenum usage)
	{
		Bind();
		glCheck(glBufferData(mTarget, size, data, usage));
		mHasData = true;
	}

}