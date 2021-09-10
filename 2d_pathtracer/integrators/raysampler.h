#pragma once

#include <iostream>
#include <memory>


class Ray;
class Scene;

class RaySampler
{
public:
	virtual ~RaySampler() = default;

	void set_scene(std::shared_ptr<Scene> scene)
	{
		m_scene = scene;
	}

	virtual void sample(const Ray& ray)  = 0;

protected:
	std::shared_ptr<Scene> m_scene;
};
