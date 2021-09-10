#pragma once

#include "../geometry/ray.hpp"
#include "../geometry/2dtypes.hpp"
#include "../geometry/intersections.hpp"
#include <iostream>

static void test_intersection()
{
	glm::vec2 rayOrigin(0.0f,0.0f);
	glm::vec2 rayDir(1.0f,1.0f);
	Ray ray(rayOrigin,rayDir);

	glm::vec2 a(20.0f,0.0f);
	glm::vec2 b(20.0f,50.0f);
	Segment s(a,b);


	Intersection isect;
    
	
	glm::vec2 sT = s.b - s.a;
	glm::vec2 sN = glm::vec2(-sT.y, sT.x);
    float t = glm::dot(sN, s.a - ray.origin)/dot(sN, ray.direction);
    float u = glm::dot(sT, ray.origin + ray.direction*t - s.a);
    if (t < isect.t_min || t >= isect.t_max || u < 0.0 || u > dot(sT, sT)){
		return;
	}
	
    isect.t_max = t;
    isect.normal = normalize(sN);
    
	
}