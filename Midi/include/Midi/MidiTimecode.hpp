#pragma once

#include <functional>

#include "CallbackInfo/BaseCallbackInfo.hpp"

#include "RtMidi.h"

namespace Wor::Midi {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class MidiTimecode {
	public:
		/**
		 * @brief Ctor.
		 */
		MidiTimecode() noexcept;

		/**
		 * @brief Dtor.
		 */
		virtual ~MidiTimecode() noexcept = default;

		/**
		 * @brief
		 *
		 * @param port
		 *
		 * @param portName
		 */
		void open(std::uint16_t port = 0, const std::string& portName = "Midi client") noexcept;

		/**
		 * @brief
		 */
		void close() noexcept;

	private:
		RtMidiIn _device;

		std::function<void(const CallbackInfo::BaseCallbackInfo&)> _callback;

#pragma region Accessors/Mutators

	public:
		/**
		 * @brief
		 *
		 * @param callback
		 */
		void callback(std::function<void(const CallbackInfo::BaseCallbackInfo&)> callback) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		bool isOpen() const noexcept;

#pragma endregion Accessors/Mutators
	};
}
