#pragma once

#include "raysampler.h"
#include "../framework.h"

struct DrawData;

struct PathtracerSettings
{
	int path_length = 5;
	bool pure_importance = false;
	bool direct_light_ray = false;
	float exposure = 1.0f;
	bool timelapse = false;
};

class Pathtracer : public RaySampler
{
public:
	Pathtracer(int width, int height, const gpupro::Program& path_program);

	void sample(const Ray& ray) override;

	void draw_result(gpupro::Program& compose_program);

	/// <summary>
	/// clears samples texture and num_iterations to 0
	/// </summary>
	void reset();

	//Settings for the Pathtracer
	PathtracerSettings settings;

private:
	//RGB32F texture where the lines are drawn
	gpupro::Texture samples_tex;
	gpupro::Framebuffer samples_framebuffer;
	//use additive blending
	gpupro::Pipeline add_samples_pipeline;
	//Shader to draw the paths
	const gpupro::Program& path_program;
	int num_iterations;

	//collect lines to draw 
	std::vector<DrawData> draw_data;

	const float RAY_EPSILON = 1e-2f;
};


struct PathSegment
{
	PathSegment(const glm::vec2& _origin,
		const glm::vec2& _destination,
		const glm::vec3& _reflectance,
		const glm::vec3& _illumination) : origin(_origin),
		destination(_destination),
		reflectance(_reflectance),
		illumination(_illumination)
	{
	}

	glm::vec2 origin;
	glm::vec2 destination;
	glm::vec3 reflectance;
	glm::vec3 illumination;
};

// data structure for line data uploaded to gpu
struct DrawData
{
	DrawData(const glm::vec2& _start, const glm::vec2& _end, const glm::vec3& _start_flux) :
		start_point(_start),
		end_point(_end),
		start_flux(_start_flux)
	{
	}

	glm::vec2 start_point;
	glm::vec2 end_point;
	glm::vec3 start_flux;
};
