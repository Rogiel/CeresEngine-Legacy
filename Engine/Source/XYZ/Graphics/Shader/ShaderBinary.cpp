//
// Created by Rogiel Sulzbach on 7/23/17.
//

#include "ShaderBinary.hpp"

namespace XYZ::Graphics::Shader {

	ShaderBinary::ShaderBinary() = default;

	ShaderBinary::ShaderBinary(ShaderBinary::BinaryBuffer binary) :
			binary(std::move(binary)) {}

	ShaderBinary::ShaderBinary(const char* binary, size_t length) :
			binary(binary, binary + length) {}

	ShaderBinary::ShaderBinary(const ShaderBinary& other) = default;
	ShaderBinary& ShaderBinary::operator=(const ShaderBinary& other) = default;
	ShaderBinary::ShaderBinary(ShaderBinary&& other) = default;
	ShaderBinary& ShaderBinary::operator=(ShaderBinary&& other) = default;

	// -----------------------------------------------------------------------------------------------------------------

	bool ShaderBinary::isValid() const {
		return binary.size() != 0;
	}

	ShaderBinary::operator bool() const {
		return binary.size() != 0;
	}

	// -----------------------------------------------------------------------------------------------------------------

	const ShaderBinary::BinaryBuffer& ShaderBinary::getBinary() const {
		return binary;
	}

	size_t ShaderBinary::getSize() const {
		return binary.size();
	}

}