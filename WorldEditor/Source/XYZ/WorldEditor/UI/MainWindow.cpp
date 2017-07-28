//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <iostream>
#include <QFileDialog>
#include <XYZ/Scene/Light/PointLight.hpp>

namespace XYZ::WorldEditor::UI {

	MainWindow::MainWindow(QWidget* parent) :
			QMainWindow(parent), ui(new Ui_MainWindow) {
		ui->setupUi(this);

//		auto gl = new QOpenGLWidget(ui->gameView);
	}

	MainWindow::~MainWindow() {
		delete ui;
	}

	// -----------------------------------------------------------------------------------------------------------------

	void MainWindow::on_actionNew_triggered() {

	}

	void MainWindow::on_actionLoad_triggered() {
		QFileDialog fileDialog(this, "Open File", "/", "*.xml");
		fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
		fileDialog.setOptions(QFileDialog::HideNameFilterDetails | QFileDialog::ReadOnly);
		fileDialog.setNameFilterDetailsVisible(false);
		fileDialog.setDefaultSuffix("xml");
		if (fileDialog.exec() == QDialog::Accepted) {
			QString fullPath = fileDialog.selectedFiles()[0];
			std::cout << fullPath.toStdString() << std::endl;
		}
	}

	void MainWindow::on_actionSave_triggered() {
		QFileDialog fileDialog(this, "Open File", "/", "*.xml");
		fileDialog.setAcceptMode(QFileDialog::AcceptSave);
		fileDialog.setOptions(QFileDialog::HideNameFilterDetails | QFileDialog::ReadOnly);
		fileDialog.setNameFilterDetailsVisible(false);
		fileDialog.setDefaultSuffix("xml");
		if (fileDialog.exec() == QDialog::Accepted) {
			QString fullPath = fileDialog.selectedFiles()[0];
			std::cout << fullPath.toStdString() << std::endl;
		}
	}

	void MainWindow::on_actionSaveAs_triggered() {

	}

	void MainWindow::on_actionDelete_triggered() {

	}

	void MainWindow::on_actionUndo_triggered() {

	}

	void MainWindow::on_actionDuplicate_triggered() {

	}

	void MainWindow::on_actionGroup_triggered() {

	}

	void MainWindow::on_actionAddModelEntity_triggered() {
		ui->viewport->getScene().getCamera()->position = glm::vec3(0.0f, 2.0f, 0.0f);
		ui->viewport->repaint();
	}

	void MainWindow::on_actionAddLightEntity_triggered() {
		auto pointLight1 = std::make_shared<Scene::Light::PointLight>();
		pointLight1->setPosition(glm::vec3(0.0, 0.0, 0.0));
		pointLight1->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
		pointLight1->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		pointLight1->setConstant(1.0f);
		pointLight1->setLinear(0.09f);
		pointLight1->setQuadratic(0.032f);

		ui->viewport->getScene().addLight(pointLight1);
		ui->viewport->repaint();

	}

	void MainWindow::on_actionAbout_triggered() {

	}

	void MainWindow::on_actionExit_triggered() {
		this->close();
	}

}