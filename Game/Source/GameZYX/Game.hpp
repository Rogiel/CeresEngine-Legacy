//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include <XYZ/Graphics/Renderer/Renderer.hpp>
#include "Input/FirstResponder.hpp"

namespace GameZYX {

	class Game {
	private:
		/**
		 * The game rendering engine
		 */
		std::unique_ptr<XYZ::Graphics::Renderer::Renderer> renderer;

		/**
		 * The game first responder
		 */
		Input::FirstResponder firstResponder;

	public:
		/**
		 * Create a new game instance
		 */
		Game();

		/**
		 * Destroys the game instance
		 */
		~Game();

	public:
		/**
		 * @return the game rendering engine
		 */
		const XYZ::Graphics::Renderer::Renderer& getRenderer() const;

		/**
		 * @return the game first responder
		 */
		const Input::FirstResponder& getFirstResponder() const;

	};

}

