#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
	glm::vec2 origin;
	glm::vec2 direction; ///< Normalized direction vector.

	/// \brief Create uninitialized ray.
	Ray() noexcept
	{
	}

	/// \brief Create from point (origin) and direction
	/// \param [in] _direction Normalized direction vector.
	Ray(const glm::vec2& _origin, const glm::vec2& _direction) noexcept : origin(_origin),
	                                                                      direction(_direction)
	{
	}
};
