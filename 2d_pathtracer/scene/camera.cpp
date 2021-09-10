#include "camera.hpp"

#include "../integrators/raysampler.h"
#include "../geometry/ray.hpp"
#include "../geometry/2dmath.hpp"

void Camera::expose(RaySampler& ray_sampler, int num_iterations)
{
	//stepsize is size of each "pixel" segment
	const float stepsize = m_fov / static_cast<float>(resolution);

	for (int i = 0; i < num_iterations; ++i)
	{
		float upper_angle = m_fov / 2;
		float lower_angle = upper_angle - stepsize;
		for (int j = 0; j < resolution; ++j)
		{
			float xi = rng.next();
			//choose random value between upper_angle and lower_angle (linear interpolation?) (jittering)
			//std::lerp(upper_angle,lower_angle,xi);
			const float random_angle = upper_angle + xi * (lower_angle - upper_angle);

			Ray ray;
			ray.origin = this->pos;
			//rotate camera.dir by random angle
			ray.direction = glm::normalize(rotate(this->get_dir(), random_angle));


			ray_sampler.sample(ray);

			upper_angle = lower_angle;
			lower_angle -= stepsize;
		}
	}
}

bool Camera::is_point_inside(glm::vec2 point) const
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
