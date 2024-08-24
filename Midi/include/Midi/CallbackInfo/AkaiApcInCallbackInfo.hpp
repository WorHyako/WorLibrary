#pragma once

#include "ICallbackInfo.hpp"

namespace Wor::Midi::CallbackInfo {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class AkaiApcInCallbackInfo final
			: public IInCallbackInfo {
	public:
		/**
		 * @brief Ctor.
		 *
		 * @param buttonId
		 *
		 * @param type
		 *
		 * @param value
		 */
		AkaiApcInCallbackInfo(std::byte buttonId,
							  KeyEventType type,
							  std::byte value) noexcept;

	private:
		KeyEventType _type;

		std::byte _buttonId;

		std::byte _value;

#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		KeyEventType type() const noexcept override;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::byte value() const noexcept override;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::byte buttonId() const noexcept override;

#pragma endregion Accessors/Mutators
	};
}
