//
// Created by Rogiel Sulzbach on 8/6/17.
//

#pragma once

#include <glm/vec3.hpp>

namespace XYZ::Graphics::Model {

	class LevelOfDetail {
	public:
		glm::vec3 screenSize;

	public:
		LevelOfDetail(glm::vec3 screenSize);

		LevelOfDetail(const LevelOfDetail& other);
		LevelOfDetail& operator=(const LevelOfDetail& other);

	};

}

