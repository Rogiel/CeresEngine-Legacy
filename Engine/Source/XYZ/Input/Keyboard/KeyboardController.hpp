//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/InputController.hpp"

#include "XYZ/Input/Keyboard/Keyboard.hpp"
#include <memory>

namespace XYZ::Input::Keyboard {

	class KeyboardController : public InputController {
	public:
		/**
		 * Virtual destructor
		 */
		~KeyboardController() override;

	public:
		/**
		 * @return the systems primary keyboard
		 */
		virtual std::shared_ptr<Keyboard> getPrimaryKeyboard() = 0;

	};

}

