#pragma once

#include "material.hpp"

class Mirror final :public  Material
{
public:
	glm::vec3 operator()(const glm::vec2& _incident, const glm::vec2& _excident, const glm::vec2& _normal) const override
	{
		return glm::vec3(1.0f) * reflection_color;
	}

	glm::vec2 sample_dir(const glm::vec2& _incident, const glm::vec2& _normal, float& _probability)  override
	{
		return glm::vec2(-_incident.x, _incident.y);
	}

	Mirror(glm::vec3 _color) : Material(_color)
	{}
};