//
// Created by Rogiel Sulzbach on 7/25/17.
//

#pragma once

#include "XYZ/Engine.hpp"
#include "XYZ/Graphics/Renderer/OpenGL/OpenGLDeferredRendering.hpp"

#include <QOpenGLWidget>
#include <QOpenGLFramebufferObject>

namespace XYZ::WorldEditor::UI {

	class EditorViewport : public QOpenGLWidget {
	private:
		Engine* engine;
		Graphics::Renderer::OpenGL::OpenGLDeferredRendering* rendering;
		Scene::Scene scene;

	public:
		EditorViewport(QWidget* parent = nullptr);

	protected:
		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;

	public:
		Scene::Scene& getScene();
		Engine* getEngine();

	public:
	protected:
		void mousePressEvent(QMouseEvent* event) override;

		void mouseMoveEvent(QMouseEvent* event) override;

		void wheelEvent(QWheelEvent* event) override;

	};

}

