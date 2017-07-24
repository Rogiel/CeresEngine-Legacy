//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "FirstResponder.hpp"

namespace GameZYX::Input {

	FirstResponder::FirstResponder(Game* game) :
			game(game) {

	}

	FirstResponder::~FirstResponder() = default;

	// -----------------------------------------------------------------------------------------------------------------

	void FirstResponder::keyPress(XYZ::Input::Keyboard::Keyboard& keyboard,
								  const XYZ::Input::Keyboard::KeyboardEvent& event) {

	}

	void FirstResponder::keyRelease(XYZ::Input::Keyboard::Keyboard& keyboard,
									const XYZ::Input::Keyboard::KeyboardEvent& event) {

	}

	// -----------------------------------------------------------------------------------------------------------------

	void FirstResponder::mouseMove(XYZ::Input::Mouse::Mouse& mouse,
								   const XYZ::Input::Mouse::MouseMoveEvent& event) {

	}

	void FirstResponder::mouseClick(XYZ::Input::Mouse::Mouse& mouse,
									const XYZ::Input::Mouse::MouseClickEvent& event) {

	}

	void FirstResponder::mouseRelease(XYZ::Input::Mouse::Mouse& mouse,
									  const XYZ::Input::Mouse::MouseClickEvent& event) {

	}

	void FirstResponder::mouseScroll(XYZ::Input::Mouse::Mouse& mouse,
									 const XYZ::Input::Mouse::MouseScrollEvent& event) {

	}
}