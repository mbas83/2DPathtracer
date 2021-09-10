#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

struct Intersection;
class Ray;
class Material;

class Primitive
{
public:
	virtual ~Primitive() = default;
	virtual bool first_intersection(const Ray& r, Intersection& isect) const = 0;

	//returns true if there is an intersection with distance at most max_dist
	virtual bool any_interscetion(const Ray& r, float max_dist) const = 0;

	virtual void set_material(std::shared_ptr<Material> material)
	{
		m_material = material;
	}

	virtual const Material& getMaterial() const { return *m_material; }
	virtual const glm::vec3& get_color() const { return color; }

	//returns vertices in screen space coordinates to draw the primitive in scene renderer
	virtual std::vector<glm::vec2> get_draw_vertices() const = 0;

	//check if a point is inside the primitive
	virtual bool is_point_inside(glm::vec2) const = 0;

	//move primitive in specified direction
	virtual void move(float dx, float dy) = 0;

protected:
	std::shared_ptr<Material> m_material;
	glm::vec3 color{ 1.0f };
};
