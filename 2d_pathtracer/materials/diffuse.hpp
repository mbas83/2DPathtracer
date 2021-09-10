#pragma once

#include <cmath>
#include "material.hpp"
#include "../utils/rng.hpp"

class Diffuse final : public Material
{
public:
	glm::vec3 operator()(const glm::vec2& _incident, const glm::vec2& _excident, const glm::vec2& _normal) const override
	{
		return glm::vec3(0.5f) * reflection_color;
	}

	glm::vec2 sample_dir(const glm::vec2& _incident, const glm::vec2& _normal, float& _probability)  override
	{
		float xi = rng.next();
		float sinThetaI = 2.0f * xi - 1.0f;
		float cosThetaI = std::sqrtf(1.0f - sinThetaI * sinThetaI);
		return glm::vec2(sinThetaI, cosThetaI * glm::sign(_incident.y));
	}

	Diffuse(glm::vec3 _color) : Material(_color),rng(0.0f, 1.0f)
	{}


private:
	RandomNumberGenerator rng;
};