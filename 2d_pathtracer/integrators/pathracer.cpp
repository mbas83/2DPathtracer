#include "pathtracer.hpp"

#include <vector>
#include <glm/glm.hpp>

#include "../scene/scene.hpp"
#include "../scene/light.hpp"
#include "../geometry/intersections.hpp"
#include "../geometry/ray.hpp"
#include "result_renderer.hpp"
#include "path_renderer.hpp"


Pathtracer::Pathtracer(int width, int height, const gpupro::Program& _path_program) :
	path_program(_path_program), num_iterations(0)
{
	add_samples_pipeline = gpupro::Pipeline();
	//set up pipeline to additive blending
	add_samples_pipeline.EnableCullFace = false; // no culling
	add_samples_pipeline.DepthMask = false;
	// disable depth writing (can be used with alpha-testing, but looks usually better to leave this on)
	add_samples_pipeline.EnableBlend = true; // normal alpha blending does not work without sorting the geometry
	add_samples_pipeline.BlendFuncSFactor = gpupro::BlendFactor::ONE;
	add_samples_pipeline.BlendFuncDFactor = gpupro::BlendFactor::ONE;

	samples_framebuffer = gpupro::Framebuffer();
	//RGB32F texture for path tracing
	samples_tex = gpupro::Texture(gpupro::TextureLayout::TEX_2D, gpupro::InternalFormat::RGB32F, 1, width, height);
	samples_framebuffer.attachColorTexture(0, samples_tex);
	samples_framebuffer.validate(); // validate once all textures were added	
}

/// 
/// \brief Samples a ray from the camera and draws into samples_tex
/// 
/// \param _ray normalized camera ray
void Pathtracer::sample(const Ray& _ray)
{
	std::vector<PathSegment> path_segments;
	bool any_hit = false;
	Ray cur_ray = _ray;

	//trace path
	for (int i = 0; i < settings.path_length; i++)
	{
		//trace current ray
		Intersection isect;

		if (m_scene->first_intersection(cur_ray, isect))
		{
			any_hit = true;

			//calculate hit position
			auto hit_pos = cur_ray.origin + cur_ray.direction * isect.t_max;


			//gather direct illumination (next event estimation)
			glm::vec3 illumination(0.0f);
			for (const auto& light : m_scene->getLights())
			{
				//direction and distance to light
				glm::vec2 light_dir = light->pos - hit_pos;
				float light_distance = glm::length(light_dir);
				light_dir /= light_distance;
				//ray from hitpos to light
				Ray light_ray(hit_pos - RAY_EPSILON * cur_ray.direction, light_dir);
				//check if light is visible
				if (!m_scene->any_intersection(light_ray, light_distance - RAY_EPSILON))
				{
					// PointLight source is visible -> compute direct illumination with
					// photometric distance law.
					float cosE = abs(glm::dot(light_dir, isect.normal)); // abs = two sided material
					glm::vec3 irradiance = light->intensity * cosE;
					float attenuation = glm::max(1.0f, light_distance);
					irradiance /= attenuation;
					illumination += irradiance;
				}

			}

			//sample new direction 
			float pdf = 1.0f;
			glm::vec2 t = glm::vec2(-isect.normal.y, isect.normal.x);
			glm::vec2 wiLocal = -glm::vec2(glm::dot(t, cur_ray.direction), glm::dot(isect.normal, cur_ray.direction));

			glm::vec2 woLocal = isect.material->sample_dir(wiLocal, isect.normal, pdf);
			//transform from local to scene space
			glm::vec new_dir = (woLocal.y * isect.normal + woLocal.x * t);

			//sample material
			const auto reflectance = (*isect.material)(-cur_ray.direction, new_dir, isect.normal) / pdf;

			//add light emitted from material (e.g. area light)
			illumination += isect.material->get_self_emitting_value(isect.normal);

			//check if we only use pure importance
			if (settings.pure_importance) {
				illumination = glm::vec3(100.0f);
			}

			//add segment
			PathSegment path_segment(cur_ray.origin, hit_pos, reflectance, illumination);
			path_segments.emplace_back(path_segment);

			//update ray, forward ray in new dir by RAY_EPSILON to prevent self-intersection
			cur_ray.origin = hit_pos + RAY_EPSILON * new_dir;
			cur_ray.direction = new_dir;
		}
		else
		{
			//Draw Direct light Ray
			//draw a line from the last hit point to the light position (if no occluder and do not draw from light to camera)
			if (settings.direct_light_ray && any_hit)
			{
				//add line to path_segments origin = cur_ray.origin, dest = light->pos, 

				//check all point lights
				for (const auto& light : m_scene->getLights())
				{
					//direction and distance to light
					glm::vec2 light_dir = light->pos - cur_ray.origin;
					float light_distance = glm::length(light_dir);
					light_dir /= light_distance;
					//ray from hitpos to light
					Ray light_ray(cur_ray.origin, light_dir);
					//check if light is visible
					if (!m_scene->any_intersection(light_ray, light_distance - RAY_EPSILON))
					{
						// PointLight source is visible -> add segment from light to hit_pos
						PathSegment path_segment(cur_ray.origin, light->pos, glm::vec3(0.1f), light->intensity);
						path_segments.emplace_back(path_segment);
					}
				}


			}


			//if ray leaves scene we do not need to add it, because this ray does not transfer light (reverse direction)
			//stop tracing
			break;
		}
	}

	//Camera ray did not hit anything
	if (path_segments.size() == 0)
	{
		return;
	}

	//first path segment has no incoming flux
	glm::vec3 incoming_flux(0.0f);
	for (auto segment_it = path_segments.rbegin(); segment_it != path_segments.rend(); ++segment_it)
	{
		incoming_flux += (*segment_it).illumination;
		glm::vec3 ray_start_flux = incoming_flux * (*segment_it).reflectance;

		//line is drawn from destination to origin (bc of reverse direction)
		glm::vec2 start_point = (*segment_it).destination;
		glm::vec2 end_point = (*segment_it).origin;

		//"Rasterization Bias"
		glm::vec2 dir = end_point - start_point;
		float biasCorrection = glm::clamp(glm::length(dir) / glm::max(glm::abs(dir.x), glm::abs(dir.y)), 1.0f, 1.414214f);

		//add to vector
		draw_data.push_back(DrawData(start_point, end_point, ray_start_flux * biasCorrection));

		float distance = glm::distance((*segment_it).destination, (*segment_it).origin);
		distance = std::max(distance, 1.0f);
		//the incoming flux for the next point
		incoming_flux /= distance;
	}


	//draw all collected path segments and clear vector
	if (draw_data.size() >= 1024)
	{
		//draw lines on samples texture
		render_path(path_program, samples_framebuffer, add_samples_pipeline, *m_scene, draw_data);
		draw_data.clear();
	}

	num_iterations++;
}

void Pathtracer::draw_result(gpupro::Program& compose_program)
{
	render_result(samples_tex, num_iterations, settings.exposure, compose_program);
}

void Pathtracer::reset()
{
	num_iterations = 0;
	int size = samples_tex.getHeight() * samples_tex.getWidth();
	std::vector<glm::vec4> data(size, glm::vec4(0.0f));
	//clear samples texture to 0
	GLfloat clearColor[3] = { 0.0f, 0.0f, 0.0f };
	glClearTexImage(samples_tex.getID(), 0, GL_RGB, GL_FLOAT, clearColor);
}
