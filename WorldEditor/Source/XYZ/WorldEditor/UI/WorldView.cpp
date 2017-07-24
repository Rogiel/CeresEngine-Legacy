//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "WorldView.hpp"

namespace XYZ::WorldEditor::UI {

	WorldView::WorldView(QWidget* parent) : QWidget(parent) {
		mainLayout = new QVBoxLayout(this);

		objectTree = new QTreeWidget(this);
		mainLayout->addWidget(objectTree);

		setLayout(mainLayout);
	}

}