#pragma once

#include <vector>

namespace Wor::Midi::CallbackInfo {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	enum class KeyEventType
			: std::uint8_t {
		/**
		 * Key pressed
		 */
		Press = 0x90,
		/**
		 * Key released
		 */
		Release = 0x80,
		/**
		 * Slide moved
		 */
		Slide = 0xB0
	};

	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class IOutCallbackInfo {
	public:
		/**
		 * @brief Dtor.
		 */
		virtual ~IOutCallbackInfo() noexcept = default;

#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::vector<std::byte> message() const noexcept = 0;

#pragma endregion Accessors/Mutators
	};

	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class IInCallbackInfo {
	public:
		/**
		 * @brief Dtor.
		 */
		virtual ~IInCallbackInfo() noexcept = default;

#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual KeyEventType type() const noexcept = 0;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::byte value() const noexcept = 0;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::byte buttonId() const noexcept = 0;

#pragma endregion Accessors/Mutators
	};
}
