#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../geometry/2dtypes.hpp"
#include "camera.hpp"

class Ray;
class Primitive;
class PointLight;
class Material;

struct TransformUniform
{
	glm::vec2 scene_size;
};

class Scene
{
public:
	Scene() = default;

	void add_primitive(const std::shared_ptr<Primitive>& _p);

	/// Add a point light
	void add_light_source(const std::shared_ptr<PointLight>& _light);

	void set_camera(std::shared_ptr<Camera> _camera);
	std::shared_ptr<Camera> get_camera() const { return m_camera; }

	const std::vector<std::shared_ptr<Primitive>>& getPrimitives() const { return m_primitives; }
	const std::vector<std::shared_ptr<PointLight>>& getLights() const { return m_lights; }

	/// Test if there is an intersection and if yes return the intersection
	/// location.
	/// \param [in] _ray The ray.
	/// \param [in,out] _isect
	///		If there is an intersection with the current model the distance is set to
	///		_isect.t_max and the material_id and normal of the Primitive.
	bool first_intersection(const Ray& _ray, Intersection& _isect);

	/// Test if there is an intersection with a primitiive with distance less than max_dist
	/// \param [in] _ray The ray.
	/// \param [in] _max_dist maximum distance between origin and the hit
	bool any_intersection(const Ray& _ray, float _max_dist);


	///transform from world space to [-1,1]
	glm::vec2 toScreenSpace(glm::vec2 vec) const;

	//set size of scene
	void set_size(float width, float height);
	glm::vec2 get_size() const { return glm::vec2(m_scene_width, m_scene_height); }

	//get transform uniform buffer
	TransformUniform getTransformUniform() const;

	void reset()
	{
		m_scene_height = 0;
		m_scene_width  = 0;
		m_primitives.clear();
		m_lights.clear();
		m_camera = nullptr;
	}

private:
	std::vector<std::shared_ptr<Primitive>> m_primitives;
	std::vector<std::shared_ptr<PointLight>> m_lights;
	std::shared_ptr<Camera> m_camera;
	float m_scene_width, m_scene_height;
};
