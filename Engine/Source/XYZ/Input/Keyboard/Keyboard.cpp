//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "Keyboard.hpp"

namespace XYZ::Input::Keyboard {

	Keyboard::~Keyboard() = default;

	// -----------------------------------------------------------------------------------------------------------------

	const std::unique_ptr<KeyboardDelegate>& Keyboard::getDelegate() const {
		return delegate;
	}

	void Keyboard::setDelegate(std::unique_ptr<KeyboardDelegate> delegate) {
		Keyboard::delegate = std::move(delegate);
	}

}