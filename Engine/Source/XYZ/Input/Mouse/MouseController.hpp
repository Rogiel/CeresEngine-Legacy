//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/InputController.hpp"

#include "XYZ/Input/Mouse/Mouse.hpp"
#include <memory>

namespace XYZ::Input::Mouse {

	class MouseController : public InputController {
	public:
		/**
		 * Virtual destructor
		 */
		~MouseController() override;

	public:
		/**
		 * @return the systems primary mouse
		 */
		virtual std::shared_ptr<Mouse> getPrimaryMouse() = 0;
	};

}

