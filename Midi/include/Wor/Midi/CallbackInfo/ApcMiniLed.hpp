#pragma once

#include <map>

#include "MidiLed.hpp"

namespace Wor::Midi::CallbackInfo::ApcMini {
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
	 * @author WorHyako
	 */
	class ApcMiniLed final
			: public MidiLed {
	public:
		/**
		 * @brief	Describes color's modes for AKAI APC MINI launchpad.
		 *
		 * @author	WorHyako
		 */
		enum class Mode
				: bool {
			/**
			 * @brief Solid color mode
			 */
			Solid = false,
			/**
			 * @brief Blinking color mode
			 */
			Blink = true
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
		 * @brief Ctor.
		 */
		ApcMiniLed() noexcept;

		/**
		 * @brief
		 */
		ApcMiniLed(Color color, Mode mode) noexcept;

		/**
		 * @brief Dtor.
		 */
		~ApcMiniLed() noexcept override = default;

#pragma region Accessors/Mutators

	public:
		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::string colorName() const noexcept override;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::string modeName() const noexcept override;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		Math::Vector::Vector3<int> rgb() const noexcept override;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		static std::vector<ApcMiniLed> availableLeds() noexcept;

#pragma endregion Accessors/Mutators
	};
}
