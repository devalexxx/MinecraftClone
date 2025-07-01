//
// Created by Alex on 30/06/2025.
//

#ifndef MCC_COMMON_UTILS_BIT_ARRAY_H
#define MCC_COMMON_UTILS_BIT_ARRAY_H

#include <vector>

namespace Mcc
{

	class BitArray
	{
		public:
			BitArray(size_t size, unsigned char stride);

			[[nodiscard]] size_t GetSize  () const;
			[[nodiscard]] size_t GetStride() const;

			void SetStride(unsigned char stride);

			[[nodiscard]] size_t Get(size_t index) const;
			void Set(size_t index, size_t value);

			template<typename Archive>
			void serialize(Archive& ar);

		private:
			std::vector<size_t> mData;

			size_t mSize;
			size_t mStride;
			size_t mValuesPer64;
			size_t mMask;
	};

}

#include "Common/Utils/BitArray.inl"

#endif