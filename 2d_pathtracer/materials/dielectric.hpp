#pragma once

#include <cmath>
#include "material.hpp"
#include "../utils/rng.hpp"

class Dielectric final : public Material
{
public:
	glm::vec3 operator()(const glm::vec2& _incident, const glm::vec2& _excident,
		const glm::vec2& _normal) const override
	{
		return glm::vec3(1.0f) * reflection_color;
	}

	glm::vec2 sample_dir(const glm::vec2& _incident, const glm::vec2& _normal, float& _probability)  override
	{
		float cosThetaT;
		float eta = _incident.y < 0.0f ? ior : 1.0f / ior;
		float Fr = dielectric_reflectance(eta, std::abs(_incident.y), cosThetaT);

		if (rng.next() < Fr)
		{
			return glm::vec2(-_incident.x, _incident.y);
		}
		else
		{
			return glm::vec2(-_incident.x * eta,
				std::sqrt(1.0f - eta * eta * _incident.x * _incident.x) * -glm::sign(_incident.y));
		}
	}

	static float dielectric_reflectance(float _eta, const float cosThetaI, float& _cosThetaT)
	{
		float sinThetaTSq = _eta * _eta * (1.0f - cosThetaI * cosThetaI);
		if (sinThetaTSq > 1.0f)
		{
			_cosThetaT = 0.0f;
			return 1.0f;
		}
		_cosThetaT = std::sqrtf(1.0f - sinThetaTSq);

		float Rs = (_eta * cosThetaI - _cosThetaT) / (_eta * cosThetaI + _cosThetaT);
		float Rp = (_eta * _cosThetaT - cosThetaI) / (_eta * _cosThetaT + cosThetaI);

		return (Rs * Rs + Rp * Rp) * 0.5f;
	}

	Dielectric(glm::vec3 _color, float _ior) : Material(_color), ior(_ior), rng(0.0f, 1.0f)
	{
	}


private:
	float ior;
	RandomNumberGenerator rng;
};
