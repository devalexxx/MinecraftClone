//
// Created by Alex on 02/09/2024.
//

#include "MinecraftLib/CommandLineStore.h"
#include <fmt/base.h>

namespace Mcc
{

	CommandLineStore::CommandLineStore(int argc, char** argv)
	{
		for (int i = 1; i < argc; ++i)
		{
			std::string_view arg(argv[i]);

			int offset = 0;
			if (arg[0] == '-' )
			{
				offset++;
				if (arg[1] == '-')
				{
					offset++;
				}

				arg = arg.substr(offset);

				if (i < (argc - 1))
				{
					if (argv[i + 1][0] != '-')
					{
						std::string val(argv[i + 1]);
						mParameters.emplace(arg, std::move(val));
						i++;
					}
					else
					{
						mFlags.emplace(arg);
					}
				}
				else
				{
					mFlags.emplace(arg);
				}
			}
		}
	}

	auto CommandLineStore::GetParameter(const std::string& parameter) const -> OptParameter
	{
		auto it = mParameters.find(parameter);
		if (it != mParameters.cend())
		{
			return ref(it->second);
		}
		else
		{
			return std::nullopt;
		}
	}

	bool CommandLineStore::HasFlag(const std::string& flag) const
	{
		return mFlags.contains(flag);
	}

}// Mcc