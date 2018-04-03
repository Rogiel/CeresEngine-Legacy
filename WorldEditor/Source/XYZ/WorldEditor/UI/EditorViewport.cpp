//
// Created by Rogiel Sulzbach on 7/25/17.
//

#include "EditorViewport.hpp"

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
#include <XYZ/Graphics/Model/StaticModel.hpp>
#include <XYZ/Audio/OpenAL/OpenALAudioSystem.hpp>

#include <XYZ/Audio/Loader/OggVorbis/OggVorbisClipLoader.hpp>

#include <QWheelEvent>

namespace XYZ::WorldEditor::UI {

	EditorViewport::EditorViewport(QWidget* parent) :
			QOpenGLWidget(parent) {
		QSurfaceFormat fmt;
		fmt.setVersion(3, 3);
		fmt.setProfile(QSurfaceFormat::CoreProfile);
		setFormat(fmt);
		QSurfaceFormat::setDefaultFormat(fmt);
	}

	std::shared_ptr<Scene::Object> loadObject(const std::string& name, Engine& engine,
											  const std::shared_ptr<Scene::Object>& parent) {
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

	void EditorViewport::initializeGL() {
		// Initialize GLEW
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK) {
			return;
		}

//		fbo = new QOpenGLFramebufferObject();

		engine = new Engine(
				std::make_shared<Resource::Locator::Bundle::BundleResourceLocator>()
		);

		engine->getMeshManager().addResourceLoader(
				std::make_unique<Graphics::Mesh::Obj::ObjMeshLoader>());
		engine->getTextureImageManager().addResourceLoader(
				std::make_unique<Graphics::Texture::Stbi::StbiTextureImageLoader>());

		auto& renderer = static_cast<Graphics::Renderer::OpenGL::OpenGLRenderer&>(engine->getRenderer());
		rendering = new Graphics::Renderer::OpenGL::OpenGLDeferredRendering(renderer);

//		GLint fbo = 0;
//		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fbo);
//		std::cout << defaultFramebufferObject() << std::endl;
//
//		renderer.getDefaultFramebuffer().framebufferID = defaultFramebufferObject();
//
		auto root = std::make_shared<Scene::Object>();
		scene.setRootObject(root);

		auto rock = loadObject("Rock", *engine, root);
		auto tunnel = root->createChild();
		for(int i = 0; i < 2; i++) {
			auto segment = loadObject("Floor", *engine, tunnel);
			segment->position.x += 4.0 * i;

			auto track = loadObject("MainRail", *engine, segment);
			static_cast<Graphics::Model::StaticModel*>(track->getModel().get())->setShininess(320.0f);

			auto pipes = loadObject("Pipes", *engine, segment);

			auto lamp = loadObject("Lamp", *engine, segment);
			auto topCables = loadObject("TopCables", *engine, segment);
			auto electricityRail = loadObject("ElectricityRail", *engine, segment);

			using glm::vec3;
			float strength = 0.3f;

			auto pointLight1 = std::make_shared<Scene::Light::PointLight>();
			pointLight1->setPosition(vec3(segment->position.x, 1.88f, 1.88f));
			pointLight1->setDiffuse(vec3(1.0f, 1.0f, 1.0f) * strength);
			pointLight1->setSpecular(vec3(1.0f, 1.0f, 1.0f) * strength);
			pointLight1->setConstant(1.0f);
			pointLight1->setLinear(0.09f);
			pointLight1->setQuadratic(0.032f);
			scene.addLight(pointLight1);

			auto pointLight2 = std::make_shared<Scene::Light::PointLight>();
			pointLight2->setPosition(vec3(segment->position.x, 1.88f, -1.88f));
			pointLight2->setDiffuse(vec3(1.0f, 1.0f, 1.0f) * strength);
			pointLight2->setSpecular(vec3(1.0f, 1.0f, 1.0f) * strength);
			pointLight2->setConstant(pointLight1->getConstant());
			pointLight2->setLinear(pointLight1->getLinear());
			pointLight2->setQuadratic(pointLight1->getQuadratic());
			scene.addLight(pointLight2);
		}

		auto camera = std::make_shared<Scene::Camera>();
		scene.setCamera(camera);
		camera->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
		camera->Yaw = 0.0;
		camera->Pitch = 0.0;
		camera->updateCameraVectors();
	}

	void EditorViewport::resizeGL(int w, int h) {
		rendering->resize(w, h);
		QOpenGLWidget::resizeGL(w, h);
	}

	void EditorViewport::paintGL() {
		auto& renderer = static_cast<Graphics::Renderer::OpenGL::OpenGLRenderer&>(engine->getRenderer());
		renderer.getDefaultFramebuffer().framebufferID = defaultFramebufferObject();

		rendering->render(scene);
	}

	Scene::Scene& EditorViewport::getScene() {
		return scene;
	}

	Engine* EditorViewport::getEngine() {
		return engine;
	}

	static QPoint point;

	void EditorViewport::mousePressEvent(QMouseEvent* event) {
		point = event->pos();
	}

	void EditorViewport::mouseMoveEvent(QMouseEvent* event) {
		float dx = float(event->pos().x() - point.x());
		float dy = float(event->pos().y() - point.y());

		if(event->buttons() & Qt::MouseButton::MiddleButton && event->modifiers() == Qt::KeyboardModifier::ShiftModifier) {
			auto position = scene.getCamera()->position;
			position -= scene.getCamera()->getRight() * dx / 100.0f;
			position += scene.getCamera()->getUp() * dy / 100.0f;
			scene.getCamera()->position = position;
		}
		if(event->buttons() & Qt::MouseButton::MiddleButton && event->modifiers() == Qt::KeyboardModifier::NoModifier) {
			scene.getCamera()->Yaw += dx / 2.0f;
			scene.getCamera()->Pitch -= dy / 4.0f;
		}

		point = event->pos();

		scene.getCamera()->updateCameraVectors();
		this->repaint();
	}

	void EditorViewport::wheelEvent(QWheelEvent* event) {
		float dy = float(event->pixelDelta().y());
		scene.getCamera()->Zoom += dy / 10.0f;

		if(scene.getCamera()->Zoom < 1.0) {
			scene.getCamera()->Zoom = 1.0;
		}

//		auto position = scene.getCamera()->position;
//		position -= scene.getCamera()->getFront() * dy / 100.0f;
//		scene.getCamera()->position = position;

		this->repaint();
	}

}