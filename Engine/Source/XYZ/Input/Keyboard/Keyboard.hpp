//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/InputDevice.hpp"
#include "XYZ/Input/Keyboard/KeyboardDelegate.hpp"
#include "XYZ/Input/Keyboard/KeyboardEvent.hpp"

#include <memory>

namespace XYZ::Input::Keyboard {

	class Keyboard : public InputDevice {
	protected:
		std::unique_ptr<KeyboardDelegate> delegate;

	public:
		~Keyboard() override;

	public:
		const std::unique_ptr<KeyboardDelegate>& getDelegate() const;
		void setDelegate(std::unique_ptr<KeyboardDelegate> delegate);

	};

}

