#pragma once

#include "../framework.h"
#include "pathtracer.hpp"


static void render_path(const gpupro::Program& _program, gpupro::Framebuffer& _framebuffer, gpupro::Pipeline& pipe,
	const Scene& scene, std::vector<DrawData> draw_data)
{
	//bind vao
	gpupro::VertexArray vao;
	//positions
	vao.addBinding(/*cpp*/ 2, /*vertex.glsl*/ 0, gpupro::VertexType::FLOAT, 2);
	//colors
	vao.addBinding(/*cpp*/ 3, /*vertex.glsl*/ 1, gpupro::VertexType::FLOAT, 3);
	vao.bind();
	//apply pipeline
	pipe.apply();
	//bind framebuffer
	_framebuffer.bind();
	//bind program
	_program.bind();

	//set transformation uniform buffer 
	gpupro::Buffer<TransformUniform> uniform_buffer(gpupro::BufferType::UNIFORM, 1);
	const auto transform_uniform = scene.getTransformUniform();
	uniform_buffer.subDataUpdate(transform_uniform);
	uniform_buffer.bindAsUniformBuffer(1);

	//create buffer for ray start flux
	gpupro::Buffer<glm::vec3> flux_buffer(gpupro::BufferType::UNIFORM, draw_data.size() * 2);

	//use Uniform Buffer as Vertex Buffer to use subDataUpdate
	auto posBuffer = gpupro::Buffer<glm::vec2>(gpupro::BufferType::UNIFORM, draw_data.size() * 2);


	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colors;
	//read all data into buffers
	for (const DrawData& data : draw_data)
	{
		//add vertex start and end position of line
		positions.emplace_back(data.start_point);
		positions.emplace_back(data.end_point);
		//add vertex color (for both vertices)
		colors.emplace_back(data.start_flux);
		colors.emplace_back(data.start_flux);
	}

	//upload to gpu
	posBuffer.subDataUpdate(positions);
	flux_buffer.subDataUpdate(colors);
	vao.bind(); // needs to be called before binding the vertex buffers
	//bind buffers
	posBuffer.bindAsVertexBuffer(2);   //number specified by vao.addBinding()
	flux_buffer.bindAsVertexBuffer(3); //number specified by vao.addBinding()
	//draw all lines
	glDrawArrays(GL_LINES, 0, posBuffer.getNumElements());
}
