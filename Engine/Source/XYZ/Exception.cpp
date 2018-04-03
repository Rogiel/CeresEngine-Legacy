//
// Created by Rogiel Sulzbach on 7/30/17.
//

#include "Exception.hpp"

namespace XYZ {

	Exception::Exception() = default;

	Exception::Exception(std::string message) : message(std::move(message)) {}

	const char* Exception::what() const noexcept {
		return message.data();
	}

}