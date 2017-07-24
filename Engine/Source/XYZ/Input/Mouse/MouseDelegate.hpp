//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

namespace XYZ::Input::Mouse {

	class Mouse;

	class MouseMoveEvent;
	class MouseClickEvent;
	class MouseScrollEvent;

	class MouseDelegate {
	public:
		virtual ~MouseDelegate() = default;

	public:
		virtual void mouseMove(Mouse& mouse, const MouseMoveEvent& event) = 0;
		virtual void mouseClick(Mouse& mouse, const MouseClickEvent& event) = 0;
		virtual void mouseRelease(Mouse& mouse, const MouseClickEvent& event) = 0;
		virtual void mouseScroll(Mouse& mouse, const MouseScrollEvent& event) = 0;

	};

}

