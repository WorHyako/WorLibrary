#pragma once

#include "ApcMiniInfo.hpp"
#include "BaseCallbackInfo.hpp"

namespace Wor::Midi::CallbackInfo {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class ApcMiniInCallbackInfo final
			: public BaseCallbackInfo {
	public:
		/**
		 * @brief Ctor.
		 *
		 * @param message
		 */
		explicit ApcMiniInCallbackInfo(std::vector<std::byte> message) noexcept;

#pragma region Accessors/Mutators

	public:
		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		ApcMini::KeyEventType type() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::uint8_t value() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::uint8_t buttonId() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		bool valid() const noexcept override;

#pragma endregion Accessors/Mutators
	};
}
