#pragma once

#include "primitive.hpp"
#include <glm/glm.hpp>

// \brief A line segment is the finite connection between two points
struct Segment : public Primitive
{
	glm::vec2 a = glm::vec2(0);
	glm::vec2 b = glm::vec2(0);

	/// \brief Create uninitialized line.
	Segment() noexcept = default;

	/// \brief Create from two points (start and end).
	Segment(const glm::vec2& _a, const glm::vec2& _b, glm::vec3 _color) noexcept : a(_a),
		b(_b)
	{
		color = _color;
	}

	bool first_intersection(const Ray& ray, Intersection& isect) const override;
	bool any_interscetion(const Ray& ray, float max_dist) const override;
	std::vector<glm::vec2> get_draw_vertices() const override;
	bool is_point_inside(glm::vec2) const override;
	void move(float dx, float dy) override;
};


struct Sphere : Primitive
{
	glm::vec2 center = glm::vec2(0);
	float radius = 0;

	/// \brief Create uninitialized triangle.
	Sphere() noexcept = default;

	/// \brief Construct from center and radius
	Sphere(const glm::vec2& _center, float _radius, glm::vec3 _color) noexcept : center(_center),
		radius(_radius)
	{
		color = _color;
	}

	bool first_intersection(const Ray& ray, Intersection& isect) const override;
	bool any_interscetion(const Ray& ray, float max_dist) const override;
	std::vector<glm::vec2> get_draw_vertices() const override;
	bool is_point_inside(glm::vec2) const override;
	void move(float dx, float dy) override;
};

struct BBox : Primitive
{
	glm::vec2 center = glm::vec2(0);
	glm::vec2 size = glm::vec2(0);

	/// \brief Create uninitialized triangle.
	BBox() noexcept = default;

	/// \brief Construct from center and size
	BBox(const glm::vec2& _center, const glm::vec2& _size, glm::vec3 _color) noexcept : center(_center),
		size(_size)
	{
		color = _color;
	}

	bool first_intersection(const Ray& ray, Intersection& isect) const override;
	bool any_interscetion(const Ray& ray, float max_dist) const override;
	std::vector<glm::vec2> get_draw_vertices() const override;
	bool is_point_inside(glm::vec2) const override;
	void move(float dx, float dy) override;
};