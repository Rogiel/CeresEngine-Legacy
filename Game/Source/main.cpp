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
#include <XYZ/Terrain/Noise/NoiseTerrain.hpp>
#include <XYZ/Terrain/Plain/PlainTerrain.hpp>

#include <thread>
#include <queue>

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

#include <XYZ/Scene/Manager/Simple/SimpleSceneManager.hpp>
#include <XYZ/Graphics/Window/GLFW/GLFWWindow.hpp>
#include <XYZ/Audio/OpenAL/OpenALAudioBuffer.hpp>
#include <XYZ/Graphics/Model/StaticModel.hpp>
#include <XYZ/Terrain/Manager/Quadtree/QuadtreeTerrainManager.hpp>

//int main() {
//	using namespace XYZ::Scene::Manager::Simple;
//
//	SimpleSceneManager sceneManager(
//			glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f),
//			glm::lookAt(
//					glm::vec3(-10.0f, 0.0f, 0.0f),       // Camera position in World Space
//					glm::vec3(0.0f, 0.0f, 0.0f),         // and looks at the origin
//					glm::vec3(0.0f, 1.0f, 0.0f)          // Head is up (set to 0,-1,0 to look upside-down)
//			)
//	);
//
//	std::cout << sceneManager.visible(glm::vec3(10.0f, 0.0f, 0.0f)) << std::endl;
//
//
//}

std::shared_ptr<Scene::Object>
loadObject(const std::string& name, Engine& engine, const std::shared_ptr<Scene::Object>& parent) {
	auto object = parent->createChild();
	auto model = std::make_shared<Graphics::Model::StaticModel>(
			engine.getMeshManager().get("Objects/" + name + "/" + name + ".obj")
	);
	object->setModel(model);

	model->setDiffuseTexture(engine.getRenderer().getTextureCompiler().compileTexture(
			*engine.getTextureImageManager().get("Objects/" + name + "/" + name + "_diffuse.png")
	));
	model->getDiffuseTexture()->setMagnificationMinificationFilter(Graphics::Texture::TextureMagnification::LINEAR,
																   Graphics::Texture::TextureMinification::NEAREST_MIPMAP_LINEAR);
	model->getDiffuseTexture()->generateMipmaps();

	model->setSpecularTexture(engine.getRenderer().getTextureCompiler().compileTexture(
			*engine.getTextureImageManager().get("Objects/" + name + "/" + name + "_specular.png")
	));
	model->getSpecularTexture()->setMagnificationMinificationFilter(Graphics::Texture::TextureMagnification::LINEAR,
																	Graphics::Texture::TextureMinification::NEAREST_MIPMAP_LINEAR);
	model->getSpecularTexture()->generateMipmaps();

	model->setNormalMap(engine.getRenderer().getTextureCompiler().compileTexture(
			*engine.getTextureImageManager().get("Objects/" + name + "/" + name + "_normal.png")
	));
	model->getNormalMap()->setMagnificationMinificationFilter(Graphics::Texture::TextureMagnification::LINEAR,
															  Graphics::Texture::TextureMinification::NEAREST_MIPMAP_LINEAR);
	model->getNormalMap()->generateMipmaps();

	model->setShininess(32.0f);
	model->setCastShadows(true);

	return object;
}

Audio::AudioBuffer* stepsBuffer;
std::shared_ptr<Audio::AudioSource> stepsSource;

int main() {
	Graphics::Window::GLFW::GLFWWindow window(1024, 768, "Game");
	window.activate();

	Audio::OpenAL::OpenALAudioSystem audioSystem;

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

	auto superRoot = std::make_shared<Scene::Object>();
	scene.setRootObject(superRoot);

//	root->scale.x = 1.0 / 180 * 2.0;
//	root->scale.z = 1.0 / 360 * 2.0;

	auto plane = engine.getMeshManager().get("Objects/GroundPlane.obj");

//	double south = -90, double north = 90, double west = -180, double east = 180

//	auto terrain = std::make_shared<Terrain::Plain::PlainTerrain>();
	auto terrain = std::make_shared<Terrain::Noise::NoiseTerrain>();

	auto thingy = loadObject("Thingy", engine, superRoot);
	thingy->setScale(glm::vec3(3.0));

	auto tunnel = superRoot->createChild();
	for(int i = 0; i < 10; i++) {
		auto segment = loadObject("Floor", engine, tunnel);
		segment->position.x += 4.0 * i;
//		segment->setShininess(0.001f);
		static_cast<Graphics::Model::StaticModel*>(segment->getModel().get())->setCastShadows(false);

		auto track = loadObject("MainRail", engine, segment);
		static_cast<Graphics::Model::StaticModel*>(track->getModel().get())->setShininess(320.0f);

		auto pipes = loadObject("Pipes", engine, segment);

		auto lamp = loadObject("Lamp", engine, segment);
		auto topCables = loadObject("TopCables", engine, segment);
		auto electricityRail = loadObject("ElectricityRail", engine, segment);

		using glm::vec3;
		float strength = 0.4f;

		auto pointLight1 = std::make_shared<Scene::Light::PointLight>();
		pointLight1->setPosition(vec3(segment->position.x, 1.8f, 1.7f));
		pointLight1->setDiffuse(vec3(1.0f, 1.0f, 1.0f) * strength);
		pointLight1->setSpecular(vec3(1.0f, 1.0f, 1.0f) * strength);
		pointLight1->setConstant(1.0f);
		pointLight1->setLinear(0.6f);
		pointLight1->setQuadratic(0.00f);
		pointLight1->setShadows(false);
		scene.addLight(pointLight1);

		auto pointLight2 = std::make_shared<Scene::Light::PointLight>();
		pointLight2->setPosition(vec3(segment->position.x, 1.8f, -1.7f));
		pointLight2->setDiffuse(vec3(1.0f, 1.0f, 1.0f) * strength);
		pointLight2->setSpecular(vec3(1.0f, 1.0f, 1.0f) * strength);
		pointLight2->setConstant(pointLight1->getConstant());
		pointLight2->setLinear(pointLight1->getLinear());
		pointLight2->setQuadratic(pointLight1->getQuadratic());
		pointLight2->setShadows(false);
		scene.addLight(pointLight2);
	}

	camera = std::make_shared<Scene::Camera>();
	scene.setCamera(camera);
	camera->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	camera->Yaw = 0.0;
	camera->Pitch = -90.0;
	camera->updateCameraVectors();

//	auto pointLight1 = std::make_shared<Scene::Light::PointLight>();
//	pointLight1->setPosition(glm::vec3(0.0, 3.0, 0.0));
//	pointLight1->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f) * 1.0f);
//	pointLight1->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
//	pointLight1->setAmbient(glm::vec3(1.0f));
//	pointLight1->setConstant(1.0f);
//	pointLight1->setLinear(0.09f * 0.0f);
//	pointLight1->setQuadratic(0.032f * 0.0f);
//	scene.addLight(pointLight1);

	auto spotLight = std::make_shared<Scene::Light::SpotLight>();
	spotLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f);
	spotLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f);
	spotLight->setConstant(1.0f);
	spotLight->setLinear(0.6f);
//	spotLight->setQuadratic(0.032f);
	spotLight->setShadowOcclusionStrength(0.9);
//	spotLight->setAmbient(glm::vec3(1.0, 1.0f, 1.0f));

	spotLight->setCutOff(20.0f);
	spotLight->setOuterCutOff(25.0f);
	scene.addLight(spotLight);

	// create a dummy audio source
	Audio::Loader::OggVorbis::OggVorbisClipLoader clipLoader;

	auto audioBuffer = audioSystem.createAudioBuffer(*clipLoader.load(
			engine.getResourceLocator().locate("Audio/Train.ogg")
	));

//	auto source = audioSystem.createSource();
//	source->play(*audioBuffer);

	auto stepsAudioBuffer = audioSystem.createAudioBuffer(*clipLoader.load(
			engine.getResourceLocator().locate("Audio/Steps.ogg")
	));

	audioSystem.getListener().setPosition(camera->getPosition());
	audioSystem.getListener().setDirection(camera->getFront());

	stepsBuffer = stepsAudioBuffer.get();
	stepsSource = audioSystem.createSource();

	std::mutex mutex;

//	double scale = 10.0;
//	for(int x = -5; x < 5; x++) {
//		for(int y = -5; y < 5; y++) {
//			auto terrainObject = superRoot->createChild();
//
//			double south = x * scale;
//			double north = south + scale;
//
//			double west = y * scale;
//			double east = west + scale;
//
//			terrainObject->setModel(terrain->createModel({south, north, west, east}));
//			terrainObject->position = glm::vec3(west, 0.0, south);
////			terrainObject->setScale(glm::vec3(float(1.0f / scale)));
//
//			std::cout << "Terrain was reloaded. SN: [" << south << ", " << north << "], WE: [" << west << ", " << east
//					  << "]"
//					  << std::endl;
//
//		}
//	}

	camera->setZFar(100.0);

	Terrain::Manager::Quadtree::QuadtreeTerrainManager terrainManager(terrain, camera);

	// update terrain
	for(auto& patch : terrainManager.getVisibleTerrainPatches()) {
		auto topLeft = (patch->getCenter() - patch->getSize() / 2.0f);
		auto bottomRight = (patch->getCenter() + patch->getSize() / 2.0f);

		auto model = terrain->createModel({topLeft.y, bottomRight.y,
										   topLeft.x, bottomRight.x});

		auto terrainObject = superRoot->createChild();
		terrainObject->setModel(model);
		terrainObject->position = glm::vec3(topLeft.x, 0.0, topLeft.y);

		std::cout << "Terrain was created. SN: [" << topLeft.y << ", " << bottomRight.y << "], WE: [" << topLeft.x
				  << ", " << bottomRight.x
				  << "]" << std::endl;
	}

//	auto terrainObject = superRoot->createChild();
	while(true) {
		auto start = glfwGetTime();

		spotLight->setPosition(camera->getPosition());
		spotLight->setDirection(camera->getFront());
		spotLight->position.y -= 0.5;

//		pointLight1->setPosition(camera->getPosition());
//		pointLight1->position.y -= 0.5;

		stepsSource->setPosition(camera->getPosition());
		stepsSource->setDirection(camera->getFront());

		audioSystem.getListener().setPosition(camera->getPosition());
		audioSystem.getListener().setDirection(camera->getFront());

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
		window.setTitle(
				"Game - " + std::to_string(fps) + " FPS / " + std::to_string(elapsed) + " seconds per frame");
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
		camera->ProcessKeyboard(Scene::Camera::FORWARD, deltaTime * 1.0f);
		walking = true;
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::BACKWARD, deltaTime * 1.0f);
		walking = true;
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::LEFT, deltaTime * 1.0f);
		walking = true;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->ProcessKeyboard(Scene::Camera::RIGHT, deltaTime * 1.0f);
		walking = true;
	}

//	if(stepsSource != nullptr) {
//		if(wasWalking == false && walking == true) {
//			stepsSource->play(*stepsBuffer);
//		}
//		if(wasWalking == true && walking == false) {
//			stepsSource->stop();
//		}
//	}

}