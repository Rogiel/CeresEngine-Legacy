//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <QMainWindow>

namespace XYZ::WorldEditor::UI {

	class Ui_MainWindow;

	class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui_MainWindow* ui;

	public:
		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow();

	private slots:

		void on_actionNew_triggered();
		void on_actionLoad_triggered();
		void on_actionSave_triggered();
		void on_actionSaveAs_triggered();

		void on_actionDelete_triggered();
		void on_actionUndo_triggered();
		void on_actionDuplicate_triggered();
		void on_actionGroup_triggered();

		void on_actionAddModelEntity_triggered();
		void on_actionAddLightEntity_triggered();

		void on_actionAbout_triggered();
		void on_actionExit_triggered();

	};
}


