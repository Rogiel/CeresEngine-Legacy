//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <QtWidgets>

namespace XYZ::WorldEditor::UI {

	class WorldView : public QWidget {
	private:
		QVBoxLayout* mainLayout;
		QTreeWidget* objectTree;

	public:
		WorldView(QWidget* parent = nullptr);

	};

}


