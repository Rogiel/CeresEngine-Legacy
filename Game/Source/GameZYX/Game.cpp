//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "Game.hpp"

namespace GameZYX {

	Game::Game() : firstResponder(this) {

	}

	Game::~Game() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	const XYZ::Graphics::Renderer::Renderer& Game::getRenderer() const {
		return *renderer;
	}

	const Input::FirstResponder& Game::getFirstResponder() const {
		return firstResponder;
	}

}