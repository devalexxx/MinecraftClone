//
// Created by Alex on 30/06/2025.
//

#include "Common/Utils/BitArray.h"

namespace Mcc
{

	BitArray::BitArray(size_t size, unsigned char stride) :
		mSize  (size),
		mStride(stride)
	{
		mValuesPer64 = (sizeof(decltype(mData)::value_type) * 8) / mStride;
		mData.resize((mSize + mValuesPer64 - 1) / mValuesPer64);
		mMask = (1ULL << mStride) - 1;
	}

	size_t BitArray::GetSize() const
	{
		return mSize;
	}

	size_t BitArray::GetStride() const
	{
		return mStride;
	}

	void BitArray::SetStride(unsigned char stride)
	{
		if (stride == mStride) return;

		BitArray newArray(mSize, stride);
		for (size_t i = 0; i < mSize; ++i)
		{
			newArray.Set(i, Get(i));
		}

		*this = std::move(newArray);
	}

	size_t BitArray::Get(size_t index) const
	{
		size_t bitIndex  = index * mStride;
		size_t dataSize  = sizeof(decltype(mData)::value_type) * 8;
		size_t dataIndex = bitIndex / dataSize;
		size_t offset    = bitIndex % dataSize;

		size_t value = mData[dataIndex] >> offset;
		if (offset + mStride > dataSize)
		{
			value |= mData[dataIndex + 1] << (dataSize - offset);
		}
		return value & mMask;
	}

	void BitArray::Set(size_t index, size_t value)
	{
		size_t bitIndex  = index * mStride;
		size_t dataSize  = sizeof(decltype(mData)::value_type) * 8;
		size_t dataIndex = bitIndex / dataSize;
		size_t offset    = bitIndex % dataSize;

		mData[dataIndex] &= ~(mMask << offset);
		mData[dataIndex] |=  (value & mMask) << offset;

		if (offset + mStride > dataSize) {
			size_t bitsInNext = (offset + mStride) - dataSize;
			mData[dataIndex + 1] &= ~(mMask >> (mStride - bitsInNext));
			mData[dataIndex + 1] |= (value & mMask) >> (mStride - bitsInNext);
		}
	}

}