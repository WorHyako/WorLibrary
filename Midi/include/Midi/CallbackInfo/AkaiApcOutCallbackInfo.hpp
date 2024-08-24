#pragma once

#include "ICallbackInfo.hpp"
#include <cstdint>

namespace Wor::Midi::CallbackInfo {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class AkaiApcOutCallbackInfo final
			: public IOutCallbackInfo {
	public:
		/**
		 * @brief
		 *
		 * @author WorHyako
		 */
		enum class ColorType
				: bool {
			Solid = true,

			Blink = false
		};

		/**
		 * @brief
		 *
		 * @author WorHyako
		 */
		enum class Color
				: std::uint8_t {
			None = 0x00,

			Green = 0x01,

			Red = 0x03,

			Yellow = 0x05,
		};

		/**
		 * @brief Ctor.
		 *
		 * @param buttonId
		 *
		 * @param color
		 *
		 * @param colorType
		 */
		AkaiApcOutCallbackInfo(std::byte buttonId,
							   Color color,
							   ColorType colorType = ColorType::Solid) noexcept;

	private:
		std::byte _color;

		std::byte _buttonId;

		const std::byte _messageId;

#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::vector<std::byte> message() const noexcept override;

#pragma endregion Accessors/Mutators
	};
}
