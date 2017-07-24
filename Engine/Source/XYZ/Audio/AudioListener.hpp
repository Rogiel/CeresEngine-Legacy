//
// Created by Rogiel Sulzbach on 7/18/17.
//

#pragma once

#include <glm/glm.hpp>

namespace XYZ::Audio {

	class AudioListener {
	private:
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 velocity;

	};

}

