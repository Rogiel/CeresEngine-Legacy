//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "Engine.hpp"

#include "XYZ/Graphics/Renderer/OpenGL/OpenGLRenderer.hpp"

#include "XYZ/Input/Keyboard/GLFW/GLFWKeyboardController.hpp"
#include "XYZ/Input/Mouse/GLFW/GLFWMouseController.hpp"

namespace XYZ {

	Engine::Engine(std::shared_ptr<Resource::Locator::ResourceLocator> resourceLocator) :
			renderer(std::make_unique<Graphics::Renderer::OpenGL::OpenGLRenderer>()),
			inputDeviceManager(std::make_unique<Input::InputDeviceManager>(
					std::make_shared<Input::Keyboard::GLFW::GLFWKeyboardController>(
							glfwGetCurrentContext()
					),
					std::make_shared<Input::Mouse::GLFW::GLFWMouseController>(
							glfwGetCurrentContext()
					)
			)),
			resourceLocator(resourceLocator),
			textureImageManager(resourceLocator, ""),
			meshManager(resourceLocator, "")
	{

	}

	Engine::~Engine() = default;

	// -----------------------------------------------------------------------------------------------------------------

	void Engine::start() {

	}

	void Engine::stop() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	Graphics::Renderer::Renderer& Engine::getRenderer() {
		return *renderer;
	}

	Input::InputDeviceManager& Engine::getInputDeviceManager() {
		return *inputDeviceManager;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Resource::Locator::ResourceLocator& Engine::getResourceLocator() const {
		return *resourceLocator;
	}

	Resource::ResourceManager<Graphics::Texture::TextureImage>& Engine::getTextureImageManager() {
		return textureImageManager;
	}

	Resource::ResourceManager<Graphics::Mesh::Mesh>& Engine::getMeshManager() {
		return meshManager;
	}

}