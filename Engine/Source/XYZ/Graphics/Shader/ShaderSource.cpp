//
// Created by Rogiel Sulzbach on 7/5/17.
//

#include <string>
#include "ShaderSource.hpp"

namespace XYZ::Graphics::Shader {

	ShaderSource::ShaderSource(std::string source) : source(std::move(source)) {}

	ShaderSource::ShaderSource(const char* source) : source(source) {}

	ShaderSource::ShaderSource(const ShaderSource& other) = default;

	ShaderSource& ShaderSource::operator=(const ShaderSource& other) = default;

	ShaderSource::ShaderSource(ShaderSource&& other) = default;

	ShaderSource& ShaderSource::operator=(ShaderSource&& other) = default;

	// -----------------------------------------------------------------------------------------------------------------

	const std::string& ShaderSource::getSource() const {
		return source;
	}

}