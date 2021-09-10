#pragma once

#include <glm/glm.hpp>
#include "../scene/scene.hpp"
#include "../geometry/primitive.hpp"
#include "../geometry/2dtypes.hpp"
#include "../scene/light.hpp"
#include "../../shared/framework/Window.h"
#include "../integrators/pathtracer.hpp"


class UI
{
public:
	UI(std::shared_ptr<Scene> scene)
	{
		m_scene = scene;
		is_moving_primitive = false;
		is_moving_light = false;
		is_moving_camera = false;
		moved_primitive = nullptr;
	}


	bool mouse_hit_object(glm::vec2 mouse_pos);
	void mouse_release();
	void move_object(float dx, float dy);
	//returns true if a valid key is pressed
	bool on_key_down(gpupro::Window::Key& k, Pathtracer& pathtracer, std::shared_ptr<Scene>& scene, int& current_scene, std::vector<std::string>& scene_names, const std::string& scenes_path);
	void rotate_camera();
	static void print_controls();

private:
	std::shared_ptr<Scene> m_scene;
	//is true if an object is clicked
	bool is_moving_primitive;
	bool is_moving_light;
	bool is_moving_camera;
	//reference to currently moved object
	std::shared_ptr<Primitive> moved_primitive;
	std::shared_ptr<PointLight> moved_light;
};


