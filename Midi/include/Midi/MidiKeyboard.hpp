#pragma once

#include <functional>

#include <Midi/CallbackInfo/AkaiApcInCallbackInfo.hpp>

#include "RtMidi.h"

namespace Wor::Midi {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class MidiKeyboard {
	public:
		/**
		 * @brief Ctor.
		 */
		MidiKeyboard() noexcept;

		/**
		 * @brief Dtor.
		 */
		virtual ~MidiKeyboard() noexcept = default;

		/**
		 * @brief
		 */
		void open(std::uint16_t port = 0, const std::string& portName = "Midi client") noexcept;

		/**
		 * @brief
		 */
		void close() noexcept;

		/**
		 * @brief
		 *
		 * @param
		 */
		void send(int buttonId, int value) noexcept;

	private:
		enum class Direction
				: bool {
			Input,
			Output
		};

		RtMidiIn _inDevice;

		RtMidiOut _outDevice;

		std::function<void(const CallbackInfo::AkaiApcInCallbackInfo&)> _callback;

#pragma region Accessors/Mutators

	public:
		/**
		 * @brief
		 *
		 * @param callback
		 */
		void callback(std::function<void(const CallbackInfo::AkaiApcInCallbackInfo&)> callback) noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void errorCallback(RtMidiErrorCallback callback) noexcept;

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
