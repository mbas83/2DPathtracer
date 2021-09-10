#include "move_objects.hpp"

#include <iostream>
#include <algorithm>

#include "../geometry/2dmath.hpp"
#include "../scene/scene.hpp"
#include "../scene/scene_loader.hpp"

/// <summary>
/// 
/// </summary>
/// \param mouse_pos Position of mouse in [-1,1]
bool UI::mouse_hit_object(glm::vec2 mouse_pos)
{
	//transform mouse_pos to scene coordinates
	auto scene_pos = (mouse_pos + 1.0f) / 2.0f * m_scene->get_size();

	//check primitives
	for (const auto& m : m_scene->getPrimitives())
	{
		if (m->is_point_inside(scene_pos))
		{
			is_moving_primitive = true;
			moved_primitive = m;
			return true;
		}

	}

	//check lights
	for (const auto& light : m_scene->getLights())
	{
		if (light->is_point_inside(scene_pos))
		{
			is_moving_light = true;
			moved_light = light;
			return true;
		}

	}


	//check camera
	if (m_scene->get_camera()->is_point_inside(scene_pos))
	{
		is_moving_camera = true;
		return true;
	}


	return false;
}

void UI::mouse_release()
{
	//set currently moving to false and reset current object
	is_moving_primitive = false;
	is_moving_camera = false;
	is_moving_light = false;
	moved_primitive = nullptr;
	moved_light = nullptr;
}

/// <summary>
/// moves object with mouse
/// </summary>
/// \param dx movement in x-direction
/// \param dy movement in y-direction
void UI::move_object(float dx, float dy)
{
	auto scene_dx = dx * m_scene->get_size().x * 0.5f;
	auto scene_dy = dy * m_scene->get_size().y * 0.5f;


	if (is_moving_primitive)
	{
		moved_primitive->move(scene_dx, scene_dy);
		return;
	}

	if (is_moving_light)
	{
		moved_light->move(scene_dx, scene_dy);
		return;
	}

	if (is_moving_camera)
	{
		m_scene->get_camera()->move(scene_dx, scene_dy);
		return;
	}

}

bool UI::on_key_down(gpupro::Window::Key& k, Pathtracer& pathtracer, std::shared_ptr<Scene>& scene, int& current_scene, std::vector<std::string>& scene_names, const std::string& scenes_path)
{

	switch (k)
	{
		//Rotate
	case gpupro::Window::Key::R:
		rotate_camera();
		return true;
		// Increase Exposure
	case gpupro::Window::Key::KP_ADD:
		pathtracer.settings.exposure += 0.1f;
		return true;
		// Decrease Exposure
	case gpupro::Window::Key::KP_SUBTRACT:
		pathtracer.settings.exposure -= 0.1f;
		return true;
	case gpupro::Window::Key::T:
		pathtracer.settings.timelapse = !pathtracer.settings.timelapse;
		return true;
	case gpupro::Window::Key::I:
		pathtracer.settings.pure_importance = !pathtracer.settings.pure_importance;
		return true;
	case gpupro::Window::Key::D:
		pathtracer.settings.direct_light_ray = !pathtracer.settings.direct_light_ray;
		return true;
	case gpupro::Window::Key::UP:
		pathtracer.settings.path_length += 1;
		return true;
	case gpupro::Window::Key::DOWN:
		pathtracer.settings.path_length = std::max(0, pathtracer.settings.path_length - 1);
		return true;
	case gpupro::Window::Key::S:
		scene->reset();
		current_scene = (current_scene + 1) % scene_names.size();
		load_scene((scenes_path + scene_names[current_scene]).c_str(), scene);
		//scene = new_scene;
		return true;
	default:
		break;
	}
	return false;
}

void UI::rotate_camera()
{
	auto cur_dir = m_scene->get_camera()->get_dir();
	auto new_dir = rotate(cur_dir, -0.174533f);
	m_scene->get_camera()->set_dir(new_dir);
}

void UI::print_controls()
{
	std::cout << "Change Exposure: +/- \n";
	std::cout << "Rotate Camera: R \n";
	std::cout << "Toggle Timelapse Mode: T \n";
	std::cout << "Toggle Pure Importance Mode: I \n";
	std::cout << "Change Path length: Up and Down Arrow \n";
	std::cout << "Change Scene : S \n";
	std::cout << "Toggle Draw Direct Light Ray: D \n";
}
