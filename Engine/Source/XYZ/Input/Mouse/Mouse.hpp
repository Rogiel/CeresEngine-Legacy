//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/InputDevice.hpp"
#include "XYZ/Input/Mouse/MouseDelegate.hpp"
#include "XYZ/Input/Mouse/MouseEvent.hpp"

#include <memory>

namespace XYZ::Input::Mouse {

	/**
	 * The Mouse class abstracts a physical hardware devices.
	 *
	 * Device position, button and scrolling changes are sent
	 * to the attached delegate.
	 */
	class Mouse : public InputDevice {
	protected:
		/**
		 * The mouse delegate
		 */
		std::unique_ptr<MouseDelegate> delegate;

	public:
		/**
		 * Virtual destructor
		 */
		~Mouse() override;

	public:
		/**
		 * @return the mouse delegate
		 */
		const std::unique_ptr<MouseDelegate>& getDelegate() const;

		/**
		 * @param delegate the mouse delegate
		 */
		void setDelegate(std::unique_ptr<MouseDelegate> delegate);
	};

}


