#pragma once

#include <glm/glm.hpp>
#include <cmath>

inline float cross2d(glm::vec2 v1, glm::vec2 v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

/// rotate 2d vec
/// \param [in] vec the vector
/// \param [in] angle the angle in radian
inline glm::vec2 rotate(const glm::vec2& vec, float angle)
{
	return glm::vec2(
		vec.x * std::cos(angle) - vec.y * std::sin(angle),
		vec.x * std::sin(angle) + vec.y * std::cos(angle));
}
