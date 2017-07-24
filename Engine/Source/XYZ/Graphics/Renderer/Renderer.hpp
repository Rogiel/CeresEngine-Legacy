//
// Created by Rogiel Sulzbach on 6/28/17.
//

#pragma once

#include "MeshCompiler.hpp"
#include "ShaderCompiler.hpp"
#include "TextureCompiler.hpp"
#include "XYZ/Graphics/Material/MaterialFactory.hpp"

#include "Framebuffer.hpp"

#include "XYZ/Scene/Scene.hpp"
#include "XYZ/Scene/Object.hpp"

namespace XYZ::Graphics::Renderer {

	/**
	 * The Renderer class represents a rendering engine implementation.
	 *
	 * A renderer can take a Scene object and convert it into a image
	 * on the computer screen. A renderer is also responsible for
	 * creating mesh, shader and texture compilers that allow creating
	 * a renderer specific object.
	 *
	 * Furthermore, a renderer can also create a framebuffer object which
	 * is very similar to a renderer but supports rendering offscreen.
	 */
	class Renderer {
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~Renderer() = default;

	public:
		/**
		 * @return the renderer's mesh compiler
		 */
		virtual MeshCompiler& getMeshCompiler() = 0;

		/**
		 * @return the renderer's shader compiler
		 */
		virtual ShaderCompiler& getShaderCompiler() = 0;

		/**
		 * @return the renderer's texture compiler
		 */
		virtual TextureCompiler& getTextureCompiler() = 0;

		/**
		 * @return the renderer specific material factory
		 */
		virtual Material::MaterialFactory& getMaterialFactory() = 0;

	public:
		/**
		 * @return the default rendering framebuffer (generally the screen framebuffer)
		 */
		virtual Framebuffer& getDefaultFramebuffer() = 0;

	public:
		/**
		 * Creates a new framebuffer object.
		 *
		 * The framebuffer is valid for as long as the parent
		 * renderer is still valid.
		 *
		 * @param width the framebuffer width
		 * @param height the framebuffer height
		 * @param withColor if true, will attach a color buffer
		 * @param withDepth if true, will attach a depth buffer
		 *
		 * @return a newly created framebuffer
		 */
		virtual Framebuffer::Ptr createFramebuffer(size_t width, size_t height, bool withColor = true,
												   bool withDepth = true) = 0;

		/**
		 * Creates a new color framebuffer object.
		 *
		 * The framebuffer is valid for as long as the parent
		 * renderer is still valid.
		 *
		 * @param width the framebuffer width
		 * @param height the framebuffer height
		 *
		 * @return a newly created framebuffer
		 */
		virtual Framebuffer::Ptr createColorFramebuffer(size_t width, size_t height) = 0;

		/**
		 * Creates a new depth framebuffer object.
		 *
		 * The framebuffer is valid for as long as the parent
		 * renderer is still valid.
		 *
		 * @param width the framebuffer width
		 * @param height the framebuffer height
		 *
		 * @return a newly created framebuffer
		 */
		virtual Framebuffer::Ptr createDepthFramebuffer(size_t width, size_t height) = 0;

	};

}


