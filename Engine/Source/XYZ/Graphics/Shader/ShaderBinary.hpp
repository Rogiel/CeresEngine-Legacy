//
// Created by Rogiel Sulzbach on 7/23/17.
//

#pragma once

#include <vector>

namespace XYZ::Graphics::Shader {

	/**
	 * A ShaderBinary is an object responsible for wrapping the binary
	 * representation of a shader (i.e. its compiled code).
	 *
	 * This shader binary object can be copied, moved and passed by
	 * reference to a shader implementation. The shader implementations
	 * will use the contents of this object to load the binary representation
	 * of the shader into the GPU.
	 *
	 * Please note that many implementations are not cross platform and
	 * binary compiled shaders can only be loaded on the same computer
	 * with the same GPU.
	 */
	class ShaderBinary {
	public:
		/**
		 * A binary buffer type
		 */
		using BinaryBuffer = std::vector<char>;

	private:
		/**
		 * The shader binary code
		 */
		BinaryBuffer binary;

	public:
		/**
		 * Creates a empty shader binary
		 */
		ShaderBinary();

		/**
		 * Creates a new shader binary from its binary representation
		 *
		 * @param binary the shader binary code
		 */
		ShaderBinary(BinaryBuffer binary);

		/**
		 * Creates a new shader binary from its binary representation
		 *
		 * @param binary the shader binary code
		 * @param length the shader binary code length
		 */
		ShaderBinary(const char* binary, size_t length);

		/**
		 * Creates a new shader binary by copying the contents of another
		 *
		 * @param other the shader binary to copy the contents from
		 */
		ShaderBinary(const ShaderBinary& other);

		/**
		 * Assigns the contents from another shader binary object.
		 *
		 * @param other the shader binary to copy the contents from
		 *
		 * @return this
		 */
		ShaderBinary& operator=(const ShaderBinary& other);

		/**
		 * Creates a new shader binary by moving the contents of another
		 *
		 * @param other the shader binary to copy the contents from
		 */
		ShaderBinary(ShaderBinary&& other);

		/**
		 * Assigns the contents from another shader binary object.
		 *
		 * @param other the shader binary to move the contents from
		 *
		 * @return this
		 */
		ShaderBinary& operator=(ShaderBinary&& other);

	public:
		/**
		 * @return true if binary representation is valid
		 */
		bool isValid() const;

		/**
		 * @return true if binary representation is valid
		 */
		operator bool() const;

	public:
		/**
		 * @return the shader binary code
		 */
		const BinaryBuffer& getBinary() const;

		/**
		 * @return the shader binary code size
		 */
		size_t getSize() const;
	};

}


