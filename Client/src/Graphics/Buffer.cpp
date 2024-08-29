//
// Created by Alex on 28/08/2024.
//

#include "Client/Graphics/VertexArray.h"
#include <Client/Graphics/Buffer.h>

#include <cassert>

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
		assert(VertexArray::IsThereAnyBound());
		glCheck(glGenBuffers(1, &mId));
		if (mId != 0)
			mIsValid = true;
	}

	void Buffer::Bind() const
	{
		assert(mIsValid);
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