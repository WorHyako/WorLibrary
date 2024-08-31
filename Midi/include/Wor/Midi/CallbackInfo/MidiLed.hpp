#pragma once

#include <string>

#include "Wor/Vector/Vector3.hpp"

namespace Wor::Midi::CallbackInfo {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class MidiLed {
	public:
		/**
		 * @brief Ctor.
		 */
		MidiLed() noexcept;

		/**
		 * @brief Dtor.
		 */
		virtual ~MidiLed() noexcept = default;

		/**
		 * @brief
		 *
		 * @param rhs
		 *
		 * @return
		 */
		bool operator==(const MidiLed &rhs) const noexcept;

	protected:
		std::uint8_t _color;

		std::uint8_t _mode;

#pragma region Accessors/Mutators

	public:
		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::uint8_t color() const noexcept;

		/**
		 * @brief
		 *
		 * @param color
		 */
		virtual void color(std::uint8_t color) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::uint8_t mode() const noexcept;

		/**
		 * @brief
		 *
		 * @param mode
		 */
		virtual void mode(std::uint8_t mode) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual Math::Vector::Vector3<int> rgb() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::string colorName() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::string modeName() const noexcept;

#pragma endregion Accessors/Mutators
	};
}
