#pragma once

#include <map>

namespace Wor::Midi::CallbackInfo::ApcMini {
	/**
	 * @brief	Describes color's modes for AKAI APC MINI launchpad.
	 *
	 * @author	WorHyako
	 */
	enum class ColorMode
			: bool {
		/**
		 * @brief Solid color mode
		 */
		Solid = true,
		/**
		 * @brief Blinking color mode
		 */
		Blink = false
	};

	/**
	 * @brief	Describes colors for AKAI APC MINI launchpad.
	 *
	 * @author	WorHyako
	 */
	enum class Color
			: std::uint8_t {
		/**
		 * @brief No color
		 */
		None = 0x00,
		/**
		 * @brief Green color
		 */
		Green = 0x01,
		/**
		 * @brief Red color
		 */
		Red = 0x03,
		/**
		 * @brief Yellow color
		 */
		Yellow = 0x05,
	};

	/**
	 * @brief	Describes type of key's event.
	 *
	 * @author	WorHyako
	 */
	enum class KeyEventType
			: std::uint8_t {
		/**
		 * @brief Error in event parsing
		 */
		None = 0x00,
		/**
		 * @brief Key pressed
		 */
		Press = 0x90,
		/**
		 * @brief Key released
		 */
		Release = 0x80,
		/**
		 * @brief Slide moved
		 */
		Slide = 0xB0
	};

	/**
	 * @brief
	 *
	 * @return
	 */
	[[nodiscard]]
	std::map<std::uint8_t, std::string> availableColors() noexcept;

	/**
	 * @brief
	 *
	 * @return
	 */
	[[nodiscard]]
	std::map<std::uint8_t, std::string> availableColorMods() noexcept;
}
