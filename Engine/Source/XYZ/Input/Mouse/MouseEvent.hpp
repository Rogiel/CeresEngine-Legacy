//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <cstdint>

namespace XYZ::Input::Mouse {

	enum class Button {
		BUTTON_1 = 0,
		BUTTON_2 = 1,
		BUTTON_3 = 2,
		BUTTON_4 = 3,
		BUTTON_5 = 4,
		BUTTON_6 = 5,
		BUTTON_7 = 6,
		BUTTON_8 = 7,
		BUTTON_LEFT = BUTTON_1,
		BUTTON_RIGHT = BUTTON_2,
		BUTTON_MIDDLE = BUTTON_3,
	};

	enum class ModifierKey : uint8_t {
		NONE = 0,
		SHIFT = (1 << 0),
		CONTROL = (1 << 1),
		COMMAND = (1 << 2),
		ALT = (1 << 2)
	};

	class MouseEvent {

	};

	class MouseMoveEvent : public MouseEvent {
	public:
		double dx;
		double dy;

	public:
		MouseMoveEvent(double dx, double dy);
	};

	class MouseClickEvent : public MouseEvent {
	public:
		Button button;
		ModifierKey modifiers;

	public:
		MouseClickEvent();
		MouseClickEvent(Button button, ModifierKey modifierKey);
	};

	class MouseScrollEvent : public MouseEvent {
	public:
		double dx;
		double dy;

	public:
		MouseScrollEvent(double dx, double dy);
	};

	inline bool operator&(ModifierKey modifierKey1, ModifierKey modifierKey2) {
		return static_cast<uint8_t>(modifierKey1) & static_cast<uint8_t>(modifierKey2);
	}

	inline ModifierKey operator|(ModifierKey modifierKey1, ModifierKey modifierKey2) {
		return ModifierKey(
				uint8_t(modifierKey1) | uint8_t(modifierKey2)
		);
	}

	inline ModifierKey operator^(ModifierKey modifierKey1, ModifierKey modifierKey2) {
		return ModifierKey(
				uint8_t(modifierKey1) ^ uint8_t(modifierKey2)
		);
	}

	inline ModifierKey operator~(ModifierKey modifierKey) {
		return ModifierKey(~uint8_t(modifierKey));
	}

}


