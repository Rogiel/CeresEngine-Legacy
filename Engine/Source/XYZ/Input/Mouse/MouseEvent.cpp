//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "MouseEvent.hpp"

namespace XYZ::Input::Mouse {

	MouseMoveEvent::MouseMoveEvent(double dx, double dy) :
			dx(dx), dy(dy) {}

	// -----------------------------------------------------------------------------------------------------------------

	MouseClickEvent::MouseClickEvent() = default;

	MouseClickEvent::MouseClickEvent(Button button, ModifierKey modifiers) :
			button(button), modifiers(modifiers) {}

	// -----------------------------------------------------------------------------------------------------------------

	MouseScrollEvent::MouseScrollEvent(double dx, double dy) :
			dx(dx), dy(dy) {}
}
