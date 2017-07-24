//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "MainWindow.hpp"
#include "ui_mainwindow.h"

#include "SidebarMenu.hpp"
#include "WorldView.hpp"
#include "ObjectProperties.hpp"

namespace XYZ::WorldEditor::UI {

	MainWindow::MainWindow(QWidget* parent) :
			QMainWindow(parent) {
		content = new QWidget(this);
		mainLayout = new QHBoxLayout(content);
		mainLayout->setContentsMargins(0, 0, 0, 0);

		sidebarMenu = new SidebarMenu(this);
		mainLayout->addWidget(sidebarMenu);

		mainContent = new WorldView(this);
		mainLayout->addWidget(mainContent);

		objectProperties = new ObjectProperties(this);
		mainLayout->addWidget(objectProperties);

		content->setLayout(mainLayout);
		setCentralWidget(content);
	}

	MainWindow::~MainWindow() { }

	void MainWindow::on_actionExit_triggered() {
		this->close();
	}

}