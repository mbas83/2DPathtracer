#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace gpupro {
	class VertexArray;
	class Program;
}

class Scene;

class SceneRenderer
{
public:
	//draw scene 
	static void draw_scene(const Scene& scene,const gpupro::Program& triangleProgram);

	static void draw_line(const std::vector<glm::vec2>& positions, const
	                      Scene& scene, const gpupro::VertexArray& vao, const gpupro::Program& triangleProgram);
	
private:
	SceneRenderer()= default;
};

