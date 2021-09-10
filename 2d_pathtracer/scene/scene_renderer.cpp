#include "scene_renderer.hpp"

#include <iostream>
#include <stdexcept>
#include <glm/glm.hpp>
#include "scene.hpp"
#include "../geometry/2dtypes.hpp"
#include "../../shared/framework/framework.h"
#include "../../shared/framework/VertexArray.h"
#include "../../shared/framework/Program.h"
#include <glad/glad.h>
#include "../geometry/2dmath.hpp"
#include "glm/gtx/string_cast.hpp"
#include "light.hpp"

//uniform to set output color of lines and light points
struct ColorUniform
{
	glm::vec3 color;
};



/// 
/// \brief Draw camera and primitives on current bound framebuffer
/// \param [in] scene Scene
void SceneRenderer::draw_scene(const Scene& scene, const gpupro::Program& triangleProgram)
{
	//set color of drawn geometry
	const auto line_color = glm::vec3(1.0f); 
	ColorUniform color_data{};
	color_data.color = line_color;
	
	gpupro::VertexArray vao;
	vao.addBinding(/*main.cpp*/ 0, /*vertex.glsl*/ 0, gpupro::VertexType::FLOAT, 2);
	vao.bind();
	triangleProgram.bind();

	//use normal pipeline
	gpupro::Pipeline line_pipe;
	line_pipe.EnableBlend = false;
	line_pipe.apply();

	//uniform to transform coordinates to [-1,1]
	gpupro::Buffer<TransformUniform> uniform_buffer(gpupro::BufferType::UNIFORM, 1);
	const auto transform_uniform = scene.getTransformUniform();
	uniform_buffer.subDataUpdate(transform_uniform);
	uniform_buffer.bindAsUniformBuffer(1);

	//uniform to set output color
	gpupro::Buffer<ColorUniform> color_uniform(gpupro::BufferType::UNIFORM, 1);
	color_uniform.subDataUpdate(color_data);
	color_uniform.bindAsUniformBuffer(2);

	for (const auto& m : scene.getPrimitives())
	{		
		std::vector<glm::vec2> positions = m->get_draw_vertices();
		
		//create Buffer with those vertices
		auto posBuffer = gpupro::Buffer<glm::vec2>(gpupro::BufferType::ARRAY, positions);
		posBuffer.bindAsVertexBuffer(0);

		color_data.color = m->get_color();
		// set color
		color_uniform.subDataUpdate(color_data);
		color_uniform.bindAsUniformBuffer(2);
		
		glDrawArrays(GL_LINE_LOOP, 0, posBuffer.getNumElements());
	}
	
	//draw Camera (two lines from camera origin to point1 and point2)
	const auto& cam = scene.get_camera();
	const auto& direction = cam->get_dir();
	const auto& origin = cam->get_pos();

	auto point1 = origin + rotate(direction, cam->get_fov() / 2);
	auto point2 = origin + rotate(direction, -cam->get_fov() / 2);


	
	std::vector<glm::vec2> positions = {
		origin,
		point1
	};
	auto posBuffer = gpupro::Buffer<glm::vec2>(gpupro::BufferType::ARRAY, positions);
	posBuffer.bindAsVertexBuffer(0);
	glDrawArrays(GL_LINES, 0, posBuffer.getNumElements());

	//second line
	positions = {
		origin,
		point2
	};
	posBuffer = gpupro::Buffer<glm::vec2>(gpupro::BufferType::ARRAY, positions);
	posBuffer.bindAsVertexBuffer(0);
	glDrawArrays(GL_LINES, 0, posBuffer.getNumElements());

	//draw lights with the color of the lights
	for (const auto& light : scene.getLights())
	{
		glm::vec3 intensity = light->intensity;
		float max_intensity = glm::max(intensity.r ,glm::max(intensity.g,intensity.b) );
		glm::vec3 draw_color = intensity/max_intensity;
		//set color uniform
		color_data.color = draw_color;
		color_uniform.subDataUpdate(color_data);
		color_uniform.bindAsUniformBuffer(2);
		
		auto lightPosBuffer = gpupro::Buffer<glm::vec2>(gpupro::BufferType::ARRAY, light->pos);
		lightPosBuffer.bindAsVertexBuffer(0);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glDrawArrays(GL_POINTS,0,1);
	}
	
}


/// 
/// \brief Draw a line with 2 vertices on current bound framebuffer
/// \param [in] positions Positions of the two vertices in object space
void SceneRenderer::draw_line(const std::vector<glm::vec2>& positions, const Scene& scene,
                              const gpupro::VertexArray& vao, const gpupro::Program& triangleProgram)
{
	if(positions.size()!=2)
	{
		throw std::invalid_argument("draw line needs 2 vertices");
	}
	
	const std::vector<glm::vec2> screen_positions = {
		positions[0],
		positions[1]
	};

	
	gpupro::Buffer<TransformUniform> uniform_buffer(gpupro::BufferType::UNIFORM, 1);
	const auto transform_uniform = scene.getTransformUniform();
	uniform_buffer.subDataUpdate(transform_uniform);
	uniform_buffer.bindAsUniformBuffer(1);
	
	auto positionBuffer = gpupro::Buffer<glm::vec2>(gpupro::BufferType::ARRAY, screen_positions);
	vao.bind(); // bind empty vao (something needs to be bound)
	positionBuffer.bindAsVertexBuffer(0);
	triangleProgram.bind(); // bind shader
	glDrawArrays(GL_LINES, 0, positionBuffer.getNumElements());
}
