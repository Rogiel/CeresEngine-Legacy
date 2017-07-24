//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <QtWidgets>

namespace XYZ::WorldEditor::UI {

	class ObjectProperties: public QWidget {
	private:
//		QFormLayout* mainLayout;

		QScrollArea* scrollArea;
		QFormLayout* scrollAreaLayout;

	public:
		ObjectProperties(QWidget* parent = nullptr);

	};

}


