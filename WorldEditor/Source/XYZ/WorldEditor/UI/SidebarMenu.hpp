//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <QtWidgets>

namespace XYZ::WorldEditor::UI {

	class SidebarMenu : public QWidget {
	private:
		QVBoxLayout* mainLayout;

		QLabel* label;
		QTreeWidget* objectTree;

	public:
		SidebarMenu(QWidget* parent = nullptr);

	};

}


