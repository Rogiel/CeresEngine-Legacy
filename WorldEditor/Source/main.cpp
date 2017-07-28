//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include <QApplication>
#include <QFile>

#include "XYZ/WorldEditor/UI/MainWindow.hpp"

using namespace XYZ::WorldEditor::UI;

int main(int argc, char** argv) {
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(Resources);

	QFile file(":/QDarkStyle/style.qss");
	file.open(QFile::OpenModeFlag::ReadOnly);
	a.setStyleSheet(file.readAll());

    MainWindow w;
    w.show();

    return a.exec();
}