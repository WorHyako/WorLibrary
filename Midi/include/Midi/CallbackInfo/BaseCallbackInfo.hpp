#pragma once

#include <vector>

namespace Wor::Midi::CallbackInfo {

	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class BaseCallbackInfo {
	public:
		/**
		 * @brief Ctor.
		 *
		 * @param message
		 */
		explicit BaseCallbackInfo(std::vector<std::byte> message) noexcept;

		/**
		 * @brief Dtor.
		 */
		virtual ~BaseCallbackInfo() noexcept = default;

	protected:
		std::vector<std::byte> _message;

#pragma region Accessors/Mutators

	public:
		/**
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual std::vector<unsigned char> vector() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		virtual bool valid() const noexcept;

#pragma endregion Accessors/Mutators
	};
}
