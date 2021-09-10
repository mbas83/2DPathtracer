#pragma once

#include <glm/glm/glm.hpp>
#include "material.hpp"

class AreaLightMaterial final : public Material
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

	glm::vec3 get_self_emitting_value(const glm::vec2& _normal) const override
	{
		return light_intensity;
	}

	AreaLightMaterial(glm::vec3 _reflection_color, glm::vec3 _light_intensity) : Material(_reflection_color), light_intensity(_light_intensity)
	{}


private:
	glm::vec3 light_intensity;

};