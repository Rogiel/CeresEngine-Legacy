//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <QtWidgets>

namespace Ui {
	class MainWindow;
}

namespace XYZ::WorldEditor::UI {
	class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		QHBoxLayout* mainLayout;

		QWidget* content;

		QWidget* sidebarMenu;
		QWidget* mainContent;
		QWidget* objectProperties;

	public:
		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow();

	private slots:

		void on_actionExit_triggered();

	};
}


