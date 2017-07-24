//
// Created by Rogiel Sulzbach on 7/9/17.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace XYZ::Graphics::Renderer::OpenGL {

	struct OpenGLCamera {
		alignas(16) glm::vec3 position;
	};

	struct OpenGLViewProjetion {
		alignas(16) glm::mat4 projection;
		alignas(16) glm::mat4 view;
		OpenGLCamera camera;
	};

}

