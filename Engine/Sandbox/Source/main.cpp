#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <XYZ/Engine.hpp>

#include <XYZ/Graphics/Renderer/OpenGL/OpenGLRenderer.hpp>
#include <XYZ/Graphics/Mesh/Obj/ObjMeshLoader.hpp>
#include <XYZ/Scene/Light/PointLight.hpp>
#include <XYZ/Scene/Light/SpotLight.hpp>
#include <XYZ/Scene/Light/DirectionalLight.hpp>
#include <XYZ/Input/Keyboard/GLFW/GLFWKeyboardController.hpp>
#include <XYZ/Input/Mouse/GLFW/GLFWMouseController.hpp>

#include <XYZ/Resource/Locator/Bundle/BundleResourceLocator.hpp>
#include <XYZ/Graphics/Texture/Stbi/StbiTextureImageLoader.hpp>

#include <XYZ/Graphics/Renderer/OpenGL/OpenGLDeferredRendering.hpp>
#include <XYZ/Audio/OpenAL/OpenALAudioSystem.hpp>

#include <XYZ/Audio/Loader/OggVorbis/OggVorbisClipLoader.hpp>

#include <XYZ/Scene/Manager/Simple/SimpleSceneManager.hpp>
#include <XYZ/Graphics/Window/GLFW/GLFWWindow.hpp>
#include <XYZ/Audio/OpenAL/OpenALAudioBuffer.hpp>
#include <XYZ/Graphics/Model/StaticModel.hpp>

#include <XYZ/Terrain/Bullet/NoiseTerrain.hpp>

using namespace XYZ;

void processInput(GLFWwindow* window);

std::shared_ptr<Scene::Camera> camera;
static float deltaTime;

class MyKeyboardDelegate : public Input::Keyboard::KeyboardDelegate {
	virtual void keyPress(Input::Keyboard::Keyboard& keyboard, const Input::Keyboard::KeyboardEvent& event) {
//		if(event.key == Input::Keyboard::Key::ESCAPE)
//			glfwSetWindowShouldClose(window, true);

		if(event.key == Input::Keyboard::Key::W)
			camera->ProcessKeyboard(Scene::Camera::FORWARD, deltaTime);
		if(event.key == Input::Keyboard::Key::S)
			camera->ProcessKeyboard(Scene::Camera::BACKWARD, deltaTime);
		if(event.key == Input::Keyboard::Key::A)
			camera->ProcessKeyboard(Scene::Camera::LEFT, deltaTime);
		if(event.key == Input::Keyboard::Key::D)
			camera->ProcessKeyboard(Scene::Camera::RIGHT, deltaTime);
	}

	virtual void keyRelease(Input::Keyboard::Keyboard& keyboard, const Input::Keyboard::KeyboardEvent& event) {

	}

	virtual void keyRepeat(Input::Keyboard::Keyboard& keyboard, const Input::Keyboard::KeyboardEvent& event) {
		keyPress(keyboard, event);
	}
};

class MyMouseDelegate : public Input::Mouse::MouseDelegate {
public:
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;

	void mouseMove(Input::Mouse::Mouse& mouse, const Input::Mouse::MouseMoveEvent& event) override {
		float x = float(event.dx);
		float y = float(event.dy);

		if(firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xoffset = x - lastX;
		float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

		lastX = x;
		lastY = y;

		camera->ProcessMouseMovement(xoffset, yoffset);
	}

	void mouseClick(Input::Mouse::Mouse& mouse, const Input::Mouse::MouseClickEvent& event) override {

	}

	void mouseRelease(Input::Mouse::Mouse& mouse, const Input::Mouse::MouseClickEvent& event) override {

	}

	void mouseScroll(Input::Mouse::Mouse& mouse, const Input::Mouse::MouseScrollEvent& event) override {

	}
};

int main() {
	Graphics::Window::GLFW::GLFWWindow window(1024, 768, "Game");
	window.activate();

	Engine engine(
			std::make_shared<Resource::Locator::Bundle::BundleResourceLocator>()
	);

	engine.getMeshManager().addResourceLoader(
			std::make_unique<Graphics::Mesh::Obj::ObjMeshLoader>());
	engine.getTextureImageManager().addResourceLoader(
			std::make_unique<Graphics::Texture::Stbi::StbiTextureImageLoader>());

	engine.getInputDeviceManager().getPrimaryMouse()->setDelegate(std::make_unique<MyMouseDelegate>());
//	engine.getInputDeviceManager().getPrimaryKeyboard()->setDelegate(std::make_unique<MyKeyboardDelegate>());

	auto& renderer = static_cast<Graphics::Renderer::OpenGL::OpenGLRenderer&>(engine.getRenderer());
	Graphics::Renderer::OpenGL::OpenGLDeferredRendering rendering(renderer);

	// tell GLFW to capture our mouse
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Scene::Scene scene;

	auto root = std::make_shared<Scene::Object>();
	scene.setRootObject(root);

//	// TODO add plane
//	auto texture = Terrain::Noise::generate();
//	auto terrainTextureImage = Graphics::Texture::TextureImage(512, 512, texture, true);
//	auto terrainTexture = renderer.getTextureCompiler().compileTexture(terrainTextureImage);

	camera = std::make_shared<Scene::Camera>();
	scene.setCamera(camera);
	camera->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	camera->Yaw = 0.0;
	camera->Pitch = 0.0;
	camera->updateCameraVectors();


	auto spotLight = std::make_shared<Scene::Light::SpotLight>();
	spotLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f);
	spotLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f);
	spotLight->setConstant(1.0f);
	spotLight->setLinear(0.09f);
	spotLight->setQuadratic(0.032f);
	spotLight->setShadowOcclusionStrength(0.9);
//	spotLight->setAmbient(glm::vec3(1.0, 1.0f, 1.0f));

	spotLight->setCutOff(20.0f);
	spotLight->setOuterCutOff(25.0f);

	scene.addLight(spotLight);

	while(true) {
		auto start = glfwGetTime();

		spotLight->setPosition(camera->getPosition());
		spotLight->setDirection(camera->getFront());
		spotLight->position.y -= 0.5;

		processInput(glfwGetCurrentContext());
		rendering.render(scene);

		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();

		if(glfwWindowShouldClose(glfwGetCurrentContext())) {
			return 0;
		}
		glFinish();

		auto end = glfwGetTime();

		float elapsed = float(end - start);
		deltaTime = elapsed;

		auto fps = int(1.0 / elapsed);
		window.setTitle("Game - " + std::to_string(fps) + " FPS / " + std::to_string(elapsed) + " seconds per frame");
	}

}

static bool walking = false;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	bool wasWalking = walking;

	walking = false;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::FORWARD, deltaTime);
		walking = true;
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::BACKWARD, deltaTime);
		walking = true;
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::LEFT, deltaTime);
		walking = true;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::RIGHT, deltaTime);
		walking = true;
	}

}