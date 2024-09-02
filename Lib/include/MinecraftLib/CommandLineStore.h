//
// Created by Alex on 02/09/2024.
//

#ifndef MINECRAFT_COMMANDLINESTORE_H
#define MINECRAFT_COMMANDLINESTORE_H

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Mcc
{

	class CommandLineStore
	{
		public:
			using OptParameter = std::optional<std::reference_wrapper<const std::string>>;

		public:
			CommandLineStore(int argc, char** argv);

			[[nodiscard]] OptParameter GetParameter(const std::string& parameter) const;
			[[nodiscard]] bool         HasFlag(const std::string& flag) const;

		private:
			std::unordered_map<std::string, std::string> mParameters;
			std::unordered_set<std::string>              mFlags;
	};

}

#endif
