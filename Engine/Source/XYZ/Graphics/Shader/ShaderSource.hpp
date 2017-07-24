//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"
#include <string>

namespace XYZ::Graphics::Shader {

	/**
	 * A ShaderSource is an object responsible for wrapping the textual
	 * representation of a shader (i.e. its source code).
	 *
	 * This shader source object can be copied, moved and passed by
	 * reference to a shader implementation. The shader implementations
	 * will use the contents of this object to compile a binary representation
	 * of the shader that can run on the GPU.
	 */
	class ShaderSource : public Resource::Resource<ShaderSource> {
	private:
		/**
		 * The shader source code
		 */
		std::string source;

	public:
		/**
		 * Creates a new shader from a string
		 *
		 * @param source the shader source code string
		 */
		ShaderSource(std::string source);

		/**
		 * Creates a new shader from a string
		 *
		 * @param source the shader source code string
		 */
		ShaderSource(const char* source);

		/**
		 * Creates a new shader source by copying the contents of another
		 *
		 * @param other the shader source to copy the contents from
		 */
		ShaderSource(const ShaderSource& other);

		/**
		 * Assigns the contents from another shader source object.
		 *
		 * @param other the shader source to copy the contents from
		 *
		 * @return this
		 */
		ShaderSource& operator=(const ShaderSource& other);

		/**
		 * Creates a new shader source by moving the contents of another
		 *
		 * @param other the shader source to copy the contents from
		 */
		ShaderSource(ShaderSource&& other);

		/**
		 * Assigns the contents from another shader source object.
		 *
		 * @param other the shader source to move the contents from
		 *
		 * @return this
		 */
		ShaderSource& operator=(ShaderSource&& other);

	public:
		/**
		 * @return the shader source code as a string
		 */
		const std::string& getSource() const;


	};

}

