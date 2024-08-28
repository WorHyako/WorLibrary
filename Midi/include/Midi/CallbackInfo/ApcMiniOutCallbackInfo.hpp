#pragma once

#include "BaseCallbackInfo.hpp"
#include "ApcMiniInfo.hpp"

namespace Wor::Midi::CallbackInfo {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class ApcMiniOutCallbackInfo final
			: public BaseCallbackInfo {
	public:
		/**
		 * @brief Ctor.
		 *
		 * @param buttonId
		 *
		 * @param led
		 */
		explicit ApcMiniOutCallbackInfo(std::uint8_t buttonId, ApcMini::ApcMiniLed led) noexcept;

#pragma region Accessors/Mutators

	public:
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
