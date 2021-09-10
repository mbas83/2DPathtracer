#pragma once

#include <glm/glm.hpp>
#include "../utils/rng.hpp"

class RaySampler;

namespace gpupro
{
	class Framebuffer;
}

class Camera
{
public:

	/// \brief Create camera at pos
	Camera(const glm::vec2& _pos, const glm::vec2& _dir, float _fov, int _resolution) noexcept : pos(_pos),
		dir(_dir),
		m_fov(_fov),
		resolution(_resolution),
		rng(0.0f, 1.0f)
	{
	}

	/// Set an opening angle
	/// \param [in] _fov Angle in radiant
	void set_fov(const float _fov) { m_fov = _fov; }

	float get_fov() const { return m_fov; }

	/// \brief generate camera rays and sample them
	void expose(RaySampler& ray_sampler, int num_iterations);
	glm::vec2 get_dir() const { return dir; }
	glm::vec2 get_pos() const { return pos; }

	void set_pos(glm::vec2 _pos) { pos = _pos; }
	void set_dir(glm::vec2 _dir) { dir = _dir; }
	void move(float dx, float dy) { pos += glm::vec2(dx, dy); }

	bool is_point_inside(glm::vec2 point) const;

private:
	glm::vec2 pos;
	glm::vec2 dir;
	//in radian
	float m_fov;
	int resolution;
	//for jittering
	RandomNumberGenerator rng;
};
