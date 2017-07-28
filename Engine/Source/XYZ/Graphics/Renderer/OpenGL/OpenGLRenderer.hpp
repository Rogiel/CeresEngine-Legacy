//
// Created by Rogiel Sulzbach on 7/2/17.
//

#pragma once

#include <unordered_map>

#include "XYZ/Graphics/Renderer/Renderer.hpp"

#include "XYZ/Graphics/Renderer/OpenGL/OpenGLCompiler.hpp"
#include "XYZ/Graphics/Renderer/OpenGL/OpenGLFramebuffer.hpp"

#include "XYZ/Scene/Object.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

    class OpenGLRenderer : public Renderer {
    private:
		/**
		 * The OpenGL compiler
		 */
        OpenGLCompiler compiler;

		/**
		 * The OpenGL default framebuffer
		 */
		OpenGLFramebuffer defaultFramebuffer;

    public:
		/**
		 * Creates a new OpenGL renderer
		 */
        OpenGLRenderer();

		/**
		 * Virtual destructor.
		 *
		 * Closes the OpenGL renderer context.
		 */
        ~OpenGLRenderer() final;

    public:
		/**
		 * @return the renderer's mesh compiler
		 */
		MeshCompiler& getMeshCompiler() final;

		/**
		 * @return the renderer's shader compiler
		 */
		ShaderCompiler& getShaderCompiler() final;

		/**
		 * @return the renderer's texture compiler
		 */
		TextureCompiler& getTextureCompiler() final;

		/**
         * @return the renderer specific material factory
         */
		Material::MaterialFactory& getMaterialFactory() final;

	public:
		/**
		 * @return the default rendering framebuffer (generally the screen framebuffer)
		 */
		OpenGLFramebuffer& getDefaultFramebuffer() final;

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
		Framebuffer::Ptr createFramebuffer(size_t width, size_t height, bool withColor = true,
										   bool withDepth = true) final;

        /**
         * Creates a new framebuffer object.
         *
         * The framebuffer is valid for as long as the parent
         * renderer is still valid.
         *
         * @param width the framebuffer width
         * @param height the framebuffer height
         *
         * @return a newly created framebuffer
         */
        Framebuffer::Ptr createColorFramebuffer(size_t width, size_t height) final;

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
		Framebuffer::Ptr createDepthFramebuffer(size_t width, size_t height) final;

	public:
		/**
		 * Creates a new texture
		 *
		 * @param width the texture width
		 * @param height the texture height
		 *
		 * @return the newly created texture
		 */
		Texture::Texture::Ptr createTexture(size_t width, size_t height);



    };

}


