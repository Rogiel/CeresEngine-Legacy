//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "SidebarMenu.hpp"

namespace XYZ::WorldEditor::UI {

	SidebarMenu::SidebarMenu(QWidget* parent) : QWidget(parent) {
		mainLayout = new QVBoxLayout(this);

		label = new QLabel(this);
		mainLayout->addWidget(label);
		label->setText("Navigation");

		objectTree = new QTreeWidget(this);
		mainLayout->addWidget(objectTree);

		setLayout(mainLayout);
		setFixedWidth(200);
	}

}