//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace XYZ::Math {

	template<typename T, uint8_t Dim>
	class Vector;

	template<typename T>
	class Vector<T, 2> : glm::vec2 {};
	using Vector2f = Vector<float, 2>;

	template<typename T>
	class Vector<T, 3> : glm::vec3 {};
	using Vector3f = Vector<float, 3>;

}