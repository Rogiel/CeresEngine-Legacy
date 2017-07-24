//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "Mouse.hpp"

namespace XYZ::Input::Mouse {

	Mouse::~Mouse() = default;

	// -----------------------------------------------------------------------------------------------------------------

	const std::unique_ptr<MouseDelegate>& Mouse::getDelegate() const {
		return delegate;
	}

	void Mouse::setDelegate(std::unique_ptr<MouseDelegate> delegate) {
		Mouse::delegate = std::move(delegate);
	}

}
