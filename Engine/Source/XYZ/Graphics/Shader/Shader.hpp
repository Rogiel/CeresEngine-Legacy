//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

#include "XYZ/Graphics/Shader/ShaderBinary.hpp"

namespace XYZ::Graphics::Shader {

	enum class ShaderType {
		/**
		 * A Vertex Shader is a piece of software that gets executed on
    	 * the GPU for every vertex of every object being rendered.
		 */
				VERTEX,

		/**
		 * A Fragment Shader (or a Pixel Shader) is a piece of software
      	 * that gets executed on the GPU for every pixel displayed on the
    	 * screen.
		 */
				FRAGMENT,

		/**
		 * A Geometry Shader is a piece of software that gets executed
    	 * on the GPU for every vertex of every object being rendered.
    	 *
    	 * The geometry shader is able to create new vertices by using
    	 * one or more vertices as a reference.
		 */
				GEOMETRY
	};

	/**
	 * A abstract class that represents a generic shader type.
	 */
	class AbstractShader {
	public:
		/**
		 * @return the shader type
		 */
		virtual ShaderType getShaderType() const = 0;

		/**
		 * @return true if the shader is a vertex shader
		 */
		bool isVertexShader() const;

		/**
		 * @return true if the fragment is a vertex shader
		 */
		bool isFragmentShader() const;

		/**
		 * @return true if the geometry is a vertex shader
		 */
		bool isGeometryShader() const;

	public:
		/**
		 * @return the shader binary
		 */
		virtual ShaderBinary getShaderBinary() const = 0;

	};

	/**
	 * A shader is a piece of software that gets executed on the
     * GPU. Shaders are used for space transforming all vertices,
     * coloring pixels and even generating new geometry directly
     * on the GPU.
     *
     * @see VertexShaer
     * @see FragmentShader
     * @see GeometryShader
	 */
	template<typename P>
	class Shader : public AbstractShader,
				   public Resource::Resource<P> {

	};

}

