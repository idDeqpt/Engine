#ifndef CONFIGS_CONTAINER_USING_HEADER
#define CONFIGS_CONTAINER_USING_HEADER

#include <Engine/Math/Vec2.hpp>
#include <unordered_map>
#include <variant>
#include <string>

namespace eng::core
{
	using ConfigValueContainer = std::variant<bool, int, float, double, std::string, mth::Vec2>;
	using ConfigsContainer = std::unordered_map<std::string, ConfigValueContainer>;
}

#endif //CONFIGS_CONTAINER_USING_HEADER