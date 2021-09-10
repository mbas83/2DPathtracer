#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include "../shared/framework/framework.h"
#include "scene/scene.hpp"
#include "glm/gtx/string_cast.hpp"
#include "integrators/pathtracer.hpp"
#include "scene/scene_loader.hpp"
#include "scene/scene_renderer.hpp"
#include "ui/move_objects.hpp"

using namespace gpupro;
using namespace glm;

using Clock = std::chrono::high_resolution_clock;

int main()
try
{
	Window::Desc d;
	d.title = "2D Pathtracer";
	d.width = 800;
	d.height = 800;
	d.resizeable = false;
	Window wnd(d);


	// create passthrough shader
	Program passthroughProgram;
	passthroughProgram.attachVertexShader(PROJECT_PATH + std::string("shader/line_vertex.glsl"));
	passthroughProgram.attachFragmentShader(PROJECT_PATH + std::string("shader/line_fragment.glsl"));
	passthroughProgram.link();

	// create compose shader
	Program composeProgram;
	composeProgram.attachVertexShader(PROJECT_PATH + std::string("shader/compose_vert.glsl"));
	composeProgram.attachFragmentShader(PROJECT_PATH + std::string("shader/compose_fragment.glsl"));
	composeProgram.link();

	//create path shader
	Program pathProgram;
	pathProgram.attachVertexShader(PROJECT_PATH + std::string("shader/path_vertex.glsl"));
	pathProgram.attachFragmentShader(PROJECT_PATH + std::string("shader/path_fragment.glsl"));
	pathProgram.link();

	VertexArray vao;

	std::vector<vec2> positions = {
		vec2(-1.0f, -1.0f),
		vec2(1.0f, 1.0f)
	};

	// create a vertex array object to specify the input layout
	vao.addBinding(/*main.cpp*/ 0, /*vertex.glsl*/ 0, VertexType::FLOAT, 4);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	int current_scene = 0;
	std::vector<std::string> scene_names = { "color_bleeding.json", "area_light.json","refraction.json", "specular_diffuse_specular.json", "veach.json" };
	auto g_scene = std::make_shared<Scene>();

	//load scene
	std::string scenes_path = R"(C:\Users\Max\Desktop\2D_Pathtracer\2d_pathtracer\test_scenes\)";
	load_scene((scenes_path + scene_names[current_scene]).c_str(), g_scene);

	//init pahtracer
	Pathtracer pathtracer(d.width, d.height, pathProgram);
	pathtracer.set_scene(g_scene);

	//pathtracer settings
	pathtracer.settings.path_length = 5;
	pathtracer.settings.direct_light_ray = false;
	pathtracer.settings.pure_importance = false;
	pathtracer.settings.exposure = 1.0f;

	UI ui(g_scene);
	//set callbacks
	bool stop_pahtracing = false;
	wnd.setMouseDownCallback([&](Window::Button, float x, float y)
		{
			if (ui.mouse_hit_object(glm::vec2(x, y)))
			{
				stop_pahtracing = true;
				// if mouse hit an object reset the pathtracing state
				pathtracer.reset();
			}
		});

	wnd.setMouseUpCallback([&](Window::Button, float x, float y)
		{
			ui.mouse_release();
			stop_pahtracing = false;
		});

	wnd.setMouseMoveCallback([&](float x, float y, float dx, float dy)
		{
			ui.move_object(dx, dy);
		});

	wnd.setKeyDownCallback([&](Window::Key key)
		{
			if (ui.on_key_down(key, pathtracer, g_scene, current_scene, scene_names, scenes_path))
			{
				pathtracer.reset();
			}
		});
	ui.print_controls();

	int num_iterations = 0;
	int iteration_stepsize = 10;

	while (wnd.isOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!stop_pahtracing) {
			//trace x iterations
			g_scene->get_camera()->expose(pathtracer, iteration_stepsize);
		}
		//draw result in default frambuffer
		gpupro::Framebuffer::bindDefaultFramebuffer();
		pathtracer.draw_result(composeProgram);
		//render scene elements over result
		SceneRenderer::draw_scene(*g_scene, passthroughProgram);
		//display framebuffer 
		wnd.swapBuffer();

		//handle mouse/key events
		wnd.handleEvents();

		//Print current settings
		printf("\rExposure: %.1f, Timelapse %d , Pure Importance Mode: %d,Draw Direct Light: %d, Path length: %d, Scene name: %s",
			pathtracer.settings.exposure, pathtracer.settings.timelapse, pathtracer.settings.pure_importance, pathtracer.settings.direct_light_ray,
			pathtracer.settings.path_length, scene_names[current_scene].c_str());

		// Timelapse Mode
		if (pathtracer.settings.timelapse)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

	return 0;
}
catch (const std::exception& _ex)
{
	std::cerr << "ERR: " << _ex.what();
	return 1;
}