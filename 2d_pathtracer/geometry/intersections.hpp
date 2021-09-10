#pragma once

#include <iostream>
#include <limits>
#include <glm/glm.hpp>
#include <memory>
#include "../materials/material.hpp"

struct Intersection
{
	glm::vec2 normal;
	std::shared_ptr<Material> material;
	float t_max;
	float t_min;
	/// \brief Create uninitialized Intersection with t_max = FLOAT_MAX
	Intersection() noexcept : normal(glm::vec2(0)), material(nullptr), t_max(std::numeric_limits<float>::max()),t_min(1e-4f)
	{
	}

	Intersection(const glm::vec2& _normal, const std::shared_ptr<Material> _material, float _tMax) noexcept : normal(_normal),
	                                                                           material(_material),
	                                                                           t_max(_tMax),
																				t_min(1e-4f)
	
	{
	}
};

