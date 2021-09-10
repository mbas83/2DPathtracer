#pragma once

#include <glm/glm.hpp>


class PointLight
{
public:
	glm::vec2 pos;
	glm::vec3 intensity;

	enum class LightType
	{
		POINT,
		AREA,
		CONE
	};


	PointLight(const glm::vec2& _pos, const glm::vec3& _intensity) noexcept : pos(_pos), intensity(_intensity)
	{
	}

	void set_pos(glm::vec2 _pos) { pos = _pos; }
	void move(float dx, float dy) { pos += glm::vec2(dx, dy); }
	bool is_point_inside(glm::vec2 point) const
	{
		const float radius = 1;
		//check radius around origin
		float d = radius * radius - ((pos.x - point.x) * (pos.x - point.x) + (pos.y - point.y) * (pos.y - point.y));
		if (d >= 0)
		{
			return true;
		}

		return false;
	}
};