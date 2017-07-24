//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Graphics/Renderer/Renderer.hpp"
#include "XYZ/Input/InputDeviceManager.hpp"

#include "XYZ/Resource/ResourceManager.hpp"

#include <memory>

namespace XYZ {

	class Engine {
	private:
		/**
		 * The engine renderer system
		 */
		std::unique_ptr<Graphics::Renderer::Renderer> renderer;

		/**
		 * The engine input device manager
		 */
		std::unique_ptr<Input::InputDeviceManager> inputDeviceManager;

	private:
		/**
		 * The engine resource locator
		 */
		std::shared_ptr<Resource::Locator::ResourceLocator> resourceLocator;

		/**
		 * The TextureImage Resource Manager
		 */
		Resource::ResourceManager<Graphics::Texture::TextureImage> textureImageManager;

		/**
		 * The Mesh Resource Manager
		 */
		Resource::ResourceManager<Graphics::Mesh::Mesh> meshManager;

	public:
		/**
		 * Creates a new engine
		 *
		 * @param the engine resource locator
		 */
		Engine(std::shared_ptr<Resource::Locator::ResourceLocator> resourceLocator);

		/**
		 * Destroys the engine
		 */
		~Engine();

	public:
		/**
		 * Starts the game engine
		 */
		void start();

		/**
		 * Stops the game engine
		 */
		void stop();

	public:
		/**
		 * @return the renderer system
		 */
		Graphics::Renderer::Renderer& getRenderer();

		/**
		 * @return the input device manager
		 */
		Input::InputDeviceManager& getInputDeviceManager();

	public:
		Resource::Locator::ResourceLocator& getResourceLocator() const;

		/**
		 * @return the TextureImage Resource Manager
		 */
		Resource::ResourceManager<Graphics::Texture::TextureImage>& getTextureImageManager();

		/**
		 * @return the Mesh Resource Manager
		 */
		Resource::ResourceManager<Graphics::Mesh::Mesh>& getMeshManager();

	};

}


