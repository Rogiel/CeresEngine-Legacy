//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once



namespace XYZ::Input::Keyboard {

	class Keyboard;
	class KeyboardEvent;

	class KeyboardDelegate {
	public:
		virtual ~KeyboardDelegate() = default;

	public:
		virtual void keyPress(Keyboard& keyboard, const KeyboardEvent& event) = 0;
		virtual void keyRelease(Keyboard& keyboard, const KeyboardEvent& event) = 0;

	};

}


