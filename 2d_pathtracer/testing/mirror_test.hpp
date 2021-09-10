#pragma once
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include "../geometry/ray.hpp"
#include "../materials/mirror.hpp"
#include "../geometry/2dtypes.hpp"
#include "../geometry/intersections.hpp"
#include "../scene/scene_renderer.hpp"
#include <vector>
#include <iostream>


/// <summary>
/// test sampling from Mirror Material (renders on current bound framebuffer)
/// </summary>
/// <param name="scene"></param>
/// <param name="vao"></param>
/// <param name="passthroughProgram"></param>
static void test_mirror(Scene& scene,gpupro::VertexArray& vao, gpupro::Program& passthroughProgram, int count)
{
	glm::vec3 throughput(1.0f);
	//test
	Mirror mir;
	
	Ray r({20,20},normalize(glm::vec2(1,1)));
	Segment s({37.5,25},{37.5,50});
	Intersection isect;

	//intersection on diffuse surface
	if(s.first_intersection(r,isect))
	{
		glm::vec2 newOrigin = r.origin + (isect.t_max*r.direction);
		std::vector<glm::vec2> positions={
			r.origin,
			newOrigin
		};
		
		SceneRenderer::draw_line(positions,scene,vao,passthroughProgram);
	}
	//parameters do not matter with diffuse
	auto scaling = mir(r.direction,r.direction,isect.normal);
	throughput*=scaling;


	glm::vec2 newOrigin = r.origin + (isect.t_max*r.direction);
	//create new directions
	for (int i = 0; i < count; ++i)
	{
		float p = 1.0f;


		glm::vec2 t = glm::vec2(-isect.normal.y, isect.normal.x);
		glm::vec2 wiLocal = -glm::vec2(glm::dot(t, r.direction), glm::dot(isect.normal, r.direction));
		//sample new direction
		glm::vec2 woLocal = mir.sample_dir(wiLocal,isect.normal,p);

		//transform from local to scene space
		glm::vec new_dir = (woLocal.y*isect.normal + woLocal.x*t);

		
		std::vector<glm::vec2> positions={
			newOrigin,
			newOrigin + new_dir
		};
		
		SceneRenderer::draw_line(positions,scene,vao,passthroughProgram);
		
	}

	
}
