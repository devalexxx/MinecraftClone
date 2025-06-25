//
// Created by Alex on 28/08/2024.
//

#include "Client/Graphics/Common.h"

namespace Mcc
{

	template<typename T>
	void Buffer::SetData(std::span<T> data, GLenum usage)
	{
		SetData((GLsizeiptr) (sizeof(T) * data.size()), data.data(), usage);
	}

}