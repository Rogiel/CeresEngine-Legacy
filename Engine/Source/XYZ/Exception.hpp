//
// Created by Rogiel Sulzbach on 7/30/17.
//

#pragma once

#include <exception>
#include <string>

namespace XYZ {

	class Exception: public std::exception {
	private:
		std::string message;

	public:
		Exception();
		explicit Exception(std::string message);

	public:
		const char* what() const noexcept final;
	};

#define XYZ_DECL_ROOT_EXCEPTION(Name) XYZ_DECL_EXCEPTION(Name, Exception)
#define XYZ_DECL_EXCEPTION(Name, Parent) class Name : public Parent { using Parent::Parent; }

}

