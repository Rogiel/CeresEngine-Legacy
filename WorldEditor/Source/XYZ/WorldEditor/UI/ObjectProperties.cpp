//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "ObjectProperties.hpp"

namespace XYZ::WorldEditor::UI {

	ObjectProperties::ObjectProperties(QWidget* parent) : QWidget(parent) {
		setLayout(new QVBoxLayout());

		scrollArea = new QScrollArea(this);
		scrollAreaLayout = new QFormLayout(scrollArea);
		scrollArea->setLayout(scrollAreaLayout);
		layout()->addWidget(scrollArea);

		auto root = new QWidget(scrollArea);
		auto layout = new QFormLayout;
		root->setLayout(layout);
		scrollArea->layout()->addWidget(root);

		for(int i = 0; i < 10; i++) {
			auto label = new QLabel(QString("Property ") + QString::fromStdString(std::to_string(i + 1)) + ":");
			auto edit = new QLineEdit(scrollArea);
			layout->addRow(label, edit);
		}

//		setLayout(mainLayout);
	}

}