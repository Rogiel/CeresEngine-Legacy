//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include <QApplication>

#include "XYZ/WorldEditor/UI/MainWindow.hpp"

using namespace XYZ::WorldEditor::UI;

int main(int argc, char** argv) {
	QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}