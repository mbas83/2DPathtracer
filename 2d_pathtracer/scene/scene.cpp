#include "scene.hpp"
#include "../geometry/primitive.hpp"
#include "light.hpp"


void Scene::add_primitive(const std::shared_ptr<Primitive> &_p)
{
	m_primitives.push_back(_p);
}

void Scene::add_light_source(const std::shared_ptr<PointLight> &_light)
{
	m_lights.push_back(_light);
}

void Scene::set_camera(std::shared_ptr<Camera> _camera)
{
	m_camera = _camera;
}

bool Scene::first_intersection(const Ray &_ray, Intersection &_isect)
{
	bool hitAny = false;
	// Test all models. After an intersection is found it is still not
	// clear if it is the closest one.
	for (auto &model : m_primitives)
	{
		// Whenever a model is hit, max_dist (in isect) is updated.
		// A new hit is only possible if it is closer -> take the new one.
		if (model->first_intersection(_ray, _isect))
		{
			hitAny = true;
		}
	}
	return hitAny;
}

bool Scene::any_intersection(const Ray& _ray, float max_dist)
{
	// Test all models if there is an intersection with distance less than max_dist
	for(auto &model : m_primitives)
	{	
		if (model->any_interscetion(_ray,max_dist))
		{
			return true;
		}
	}
	return false;
}

glm::vec2 Scene::toScreenSpace(glm::vec2 vec) const
{
	float aspect = m_scene_width / m_scene_height; //assuming width > height

	float xCoord = vec.x * 2.0f / m_scene_width - 1;
	//xCoord *= aspect;
	float yCoord = (vec.y / m_scene_height) * 2.0f - 1.0f;

	return glm::vec2(xCoord, yCoord);
}

void Scene::set_size(float width, float height)
{
	m_scene_width = width;
	m_scene_height = height;
}

TransformUniform Scene::getTransformUniform() const
{
	float scale_x = 2.0f / m_scene_width - 1.0f;
	float scale_y = 2.0f / m_scene_height - 1.0f;
	
	TransformUniform t;
	t.scene_size = glm::vec2(m_scene_width,m_scene_height);
	return t;
}
