#pragma once

#include <iomanip>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "json.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "../geometry/2dtypes.hpp"
#include "camera.hpp"
#include "../materials/diffuse.hpp"
#include "../materials/mirror.hpp"
#include "../materials/dielectric.hpp"
#include "../materials/area_light_material.hpp"

/// \brief loads scene from json file
/// 
/// \param [in] filepath Path to file
/// \param [out] scene Load scene description into scene  
static void load_scene(const std::string& filepath, const std::shared_ptr<Scene>& scene)
{
	std::ifstream i(filepath);
	nlohmann::json j;
	i >> j;

	//load all primitives
	for (auto& element : j["geometry"])
	{

		std::string type = element["type"];
		int mat_id = element["materialId"];

		glm::vec3 color = glm::vec3(1.0f);
		//check if reflection_color field exists, if not use vec3(1.0f)
		if (element.contains("color"))
		{
			auto field = element["color"];
			color = glm::vec3(field[0], field[1], field[2]);
		}


		std::shared_ptr<Primitive> primitive;

		if (type == "segment")
		{
			glm::vec2 pointA = glm::vec2(element["a"][0], element["a"][1]);
			glm::vec2 pointB = glm::vec2(element["b"][0], element["b"][1]);
			primitive = std::make_shared<Segment>(pointA, pointB, color);
		}
		else if (type == "bbox")
		{
			glm::vec2 size = glm::vec2(element["size"][0], element["size"][1]);
			glm::vec2 center = glm::vec2(element["center"][0], element["center"][1]);
			primitive = std::make_shared<BBox>(center, size, color);
		}
		else if (type == "sphere")
		{
			glm::vec2 center = glm::vec2(element["center"][0], element["center"][1]);
			float radius = element["radius"];
			primitive = std::make_shared<Sphere>(center, radius, color);
		}


		//Set Material of Primitive
		switch (mat_id)
		{
			//Diffuse
		case 1:
		{
			std::shared_ptr<Diffuse> material(std::make_shared<Diffuse>(color));
			primitive->set_material(material);
			break;
		}
		//Mirror
		case 2:
		{
			std::shared_ptr<Mirror> material(std::make_shared<Mirror>(color));
			primitive->set_material(material);
			break;
		}
		//Dilectric	
		case 3:
		{
			std::shared_ptr<Dielectric> material(std::make_shared<Dielectric>(color, 1.5));
			primitive->set_material(material);
			break;
		}
		default:
			std::cerr << "Wrong material id (no geometry added) \n";
		}

		scene->add_primitive(primitive);
	}

	//load all lights
	for (auto& element : j["lights"])
	{
		// add all point lights
		if (std::string type = element["type"]; type == "point")
		{
			auto pos = glm::vec2(element["pos"][0], element["pos"][1]);
			auto intensity = glm::vec3(element["intensity"][0], element["intensity"][1], element["intensity"][2]);
			scene->add_light_source(std::make_shared<PointLight>(pos, intensity));
		}

		// area light is a sgement primitive that has a self emitting material
		if (std::string type = element["type"]; type == "area")
		{
			auto pointA = glm::vec2(element["a"][0], element["a"][1]);
			auto pointB = glm::vec2(element["b"][0], element["b"][1]);
			auto color = glm::vec3(1.0f);
			auto intensity = glm::vec3(element["intensity"][0], element["intensity"][1], element["intensity"][2]);

			std::shared_ptr<Primitive> primitive;

			primitive = std::make_shared<Segment>(pointA, pointB, color);
			std::shared_ptr<AreaLightMaterial> material(std::make_shared<AreaLightMaterial>(color, intensity));

			primitive->set_material(material);
			scene->add_primitive(primitive);
		}
	}

	//load camera
	glm::vec2 pos = glm::vec2(j["camera"]["pos"][0], j["camera"]["pos"][1]);
	glm::vec2 dir = glm::normalize(glm::vec2(j["camera"]["direction"][0], j["camera"]["direction"][1]));
	float angle = j["camera"]["angle"];
	//convert to radian
	angle *= glm::pi<float>() / 180;
	int resolution = j["camera"]["resolution"];
	scene->set_camera(std::make_shared<Camera>(pos, dir, angle, resolution));

	//load scene width and height
	scene->set_size(j["scene_size"]["size"][0], j["scene_size"]["size"][1]);
}
