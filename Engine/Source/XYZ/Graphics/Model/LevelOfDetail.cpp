//
// Created by Rogiel Sulzbach on 8/6/17.
//

#include "LevelOfDetail.hpp"

namespace XYZ::Graphics::Model {

	LevelOfDetail::LevelOfDetail(glm::vec3 screenSize) : screenSize(screenSize) {}

	LevelOfDetail::LevelOfDetail(const LevelOfDetail& other) = default;
	LevelOfDetail& LevelOfDetail::operator=(const LevelOfDetail& other) = default;

}