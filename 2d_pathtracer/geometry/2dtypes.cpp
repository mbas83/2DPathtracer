#include "2dtypes.hpp"

#include <glm/glm.hpp>

#include "2dmath.hpp"
#include "ray.hpp"
#include "intersections.hpp"

bool Segment::first_intersection(const Ray& ray, Intersection& isect) const
{
	glm::vec2 sT = b - a;
	glm::vec2 sN = glm::vec2(-sT.y, sT.x);
	const float t = glm::dot(sN, a - ray.origin) / dot(sN, ray.direction);
	const float u = glm::dot(sT, ray.origin + ray.direction * t - a);
	if (t < isect.t_min || t >= isect.t_max || u < 0.0 || u > dot(sT, sT)) {
		return false;
	}

	isect.t_max = t;
	isect.normal = normalize(sN);
	isect.material = this->m_material;
	return true;
}

bool Segment::any_interscetion(const Ray& r, float max_dist) const
{
	Intersection i;
	i.t_max = max_dist;
	return first_intersection(r, i);
}

std::vector<glm::vec2> Segment::get_draw_vertices() const
{
	return std::vector<glm::vec2>({ a,b });
}

bool Segment::is_point_inside(glm::vec2 point) const
{
	auto ab = b - a;
	auto perpendicular = glm::normalize(glm::vec2(ab.y, -ab.x));

	glm::vec2 A = a + perpendicular;
	glm::vec2 B = b + perpendicular;
	glm::vec2 C = b - perpendicular;
	//glm::vec2 D = glm::vec2(center.x - size.x, center.y + size.y);

	auto AB = B - A;
	auto AM = point - A;
	auto BC = C - B;
	auto BM = point - B;
	auto dotABAM = glm::dot(AB, AM);
	auto dotABAB = glm::dot(AB, AB);
	auto dotBCBM = glm::dot(BC, BM);
	auto dotBCBC = glm::dot(BC, BC);

	return 0 <= dotABAM && dotABAM <= dotABAB && 0 <= dotBCBM && dotBCBM <= dotBCBC;
}

void Segment::move(float dx, float dy)
{
	a += glm::vec2(dx, dy);
	b += glm::vec2(dx, dy);
}

bool BBox::first_intersection(const Ray& ray, Intersection& isect) const
{
	const glm::vec2 ray_inv_dir = glm::vec2(1.0) / ray.direction;

	const glm::vec2 pos = ray.origin - center;
	float tx1 = (-size.x - pos.x) * ray_inv_dir.x;
	float tx2 = (size.x - pos.x) * ray_inv_dir.x;
	float ty1 = (-size.y - pos.y) * ray_inv_dir.y;
	float ty2 = (size.y - pos.y) * ray_inv_dir.y;

	float minX = glm::min(tx1, tx2), maxX = glm::max(tx1, tx2);
	float minY = glm::min(ty1, ty2), maxY = glm::max(ty1, ty2);

	float tmin = glm::max(isect.t_min, glm::max(minX, minY));
	float tmax = glm::min(isect.t_max, glm::min(maxX, maxY));

	if (tmax >= tmin) {
		auto temp = (tmin == isect.t_min) ? tmax : tmin;
		if (temp >= isect.t_max)
		{
			return false;
		}

		isect.t_max = (tmin == isect.t_min) ? tmax : tmin;
		isect.normal = isect.t_max == tx1 ? glm::vec2(-1.0, 0.0) : isect.t_max == tx2 ? glm::vec2(1.0, 0.0) :
			isect.t_max == ty1 ? glm::vec2(0.0, -1.0) : glm::vec2(0.0, 1.0);
		isect.material = this->m_material;

		return true;
	}
	return false;
}

bool BBox::any_interscetion(const Ray& ray, float max_dist) const
{
	Intersection i;
	i.t_max = max_dist;
	return first_intersection(ray, i);
}

std::vector<glm::vec2> BBox::get_draw_vertices() const
{
	std::vector<glm::vec2> vertices;
	//4 vertices
	//dl - center - size
	//dr - center.x + size.x, center.y - size.y
	//ur - center + size
	//ul - center.x - size.x, center.y + size.y
	vertices.emplace_back(center - size);
	vertices.emplace_back(glm::vec2(center.x + size.x, center.y - size.y));
	vertices.emplace_back(center + size);
	vertices.emplace_back(glm::vec2(center.x - size.x, center.y + size.y));

	return vertices;
}

bool BBox::is_point_inside(glm::vec2 point) const
{
	glm::vec2 A = center - size;
	glm::vec2 B = glm::vec2(center.x + size.x, center.y - size.y);
	glm::vec2 C = center + size;
	//glm::vec2 D = glm::vec2(center.x - size.x, center.y + size.y);
	auto AB = B - A;
	auto AM = point - A;
	auto BC = C - B;
	auto BM = point - B;
	auto dotABAM = glm::dot(AB, AM);
	auto dotABAB = glm::dot(AB, AB);
	auto dotBCBM = glm::dot(BC, BM);
	auto dotBCBC = glm::dot(BC, BC);

	return 0 <= dotABAM && dotABAM <= dotABAB && 0 <= dotBCBM && dotBCBM <= dotBCBC;
}

void BBox::move(float dx, float dy)
{
	center += glm::vec2(dx, dy);
}


bool Sphere::first_intersection(const Ray& ray, Intersection& isect) const
{
	glm::vec2 p = ray.origin - center;
	float B = glm::dot(p, ray.direction);
	float C = glm::dot(p, p) - radius * radius;
	float detSq = B * B - C;
	if (detSq >= 0.0f) {
		float det = sqrt(detSq);
		float t = -B - det;
		if (t <= isect.t_min || t >= isect.t_max)
			t = -B + det;
		if (t > isect.t_min && t < isect.t_max) {
			isect.t_max = t;
			isect.normal = glm::normalize(p + ray.direction * t);
			isect.material = this->m_material;
			return true;
		}
	}
	return false;
}

bool Sphere::any_interscetion(const Ray& ray, float max_dist) const
{
	Intersection i;
	i.t_max = max_dist;
	return first_intersection(ray, i);
}

std::vector<glm::vec2> Sphere::get_draw_vertices() const
{
	std::vector<glm::vec2> vertices;
	int num_segments = 360;

	for (int i = 0; i < num_segments; i++) {
		float theta = 2.0f * 3.1415926f * static_cast<float>(i) / static_cast<float>(num_segments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		vertices.emplace_back(glm::vec2(x + center.x, y + center.y));
	}
	return vertices;
}

bool Sphere::is_point_inside(glm::vec2 point) const
{
	float d = radius * radius - ((center.x - point.x) * (center.x - point.x) + (center.y - point.y) * (center.y - point.y));
	if (d >= 0)
	{
		return true;
	}

	return false;
}

void Sphere::move(float dx, float dy)
{
	center += glm::vec2(dx, dy);
}

