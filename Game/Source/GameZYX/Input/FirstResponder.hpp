//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <XYZ/Input/Keyboard/KeyboardDelegate.hpp>
#include <XYZ/Input/Mouse/MouseDelegate.hpp>

namespace GameZYX { class Game; }

namespace GameZYX::Input {

	class FirstResponder : public XYZ::Input::Keyboard::KeyboardDelegate,
								public XYZ::Input::Mouse::MouseDelegate {
	private:
		/**
		 * The owning game instance
		 */
		Game* game;

	public:
		/**
		 * Create a new FirstResponder
		 *
		 * @param game the owning game instance
		 */
		FirstResponder(Game* game);

		/**
		 * Destroys the first responder
		 */
		~FirstResponder() override;

	public:
		void keyPress(XYZ::Input::Keyboard::Keyboard& keyboard,
					  const XYZ::Input::Keyboard::KeyboardEvent& event) override;

		void keyRelease(XYZ::Input::Keyboard::Keyboard& keyboard,
						const XYZ::Input::Keyboard::KeyboardEvent& event) override;

	public:
		void mouseMove(XYZ::Input::Mouse::Mouse& mouse,
					   const XYZ::Input::Mouse::MouseMoveEvent& event) override;

		void mouseClick(XYZ::Input::Mouse::Mouse& mouse,
						const XYZ::Input::Mouse::MouseClickEvent& event) override;

		void mouseRelease(XYZ::Input::Mouse::Mouse& mouse,
						  const XYZ::Input::Mouse::MouseClickEvent& event) override;

		void mouseScroll(XYZ::Input::Mouse::Mouse& mouse,
						 const XYZ::Input::Mouse::MouseScrollEvent& event) override;
	};

}

