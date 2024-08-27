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
		void send(const CallbackInfo::BaseCallbackInfo& callbackInfo) noexcept;

	private:
		/**
		 * @brief
		 *
		 * @param timeStamp
		 *
		 * @param message
		 *
		 * @param user
		 */
		static void nativeInCallback(double timeStamp, std::vector<unsigned char>* message, void* user) noexcept;

		/**
		 * @brief
		 *
		 * @param error
		 *
		 * @param errorText
		 *
		 * @param user
		 */
		static void errorInCallback(RtMidiError::Type error, const std::string &errorText, void *user) noexcept;

		/**
		 * @brief
		 *
		 * @param error
		 *
		 * @param errorText
		 *
		 * @param user
		 */
		static void errorOutCallback(RtMidiError::Type error, const std::string &errorText, void *user) noexcept;

		RtMidiIn _inDevice;

		RtMidiOut _outDevice;

		std::function<void(const CallbackInfo::BaseCallbackInfo&)> _callback;

#pragma region Accessors/Mutators

	public:
		/**
		 * @brief
		 *
		 * @param callback
		 */
		void inCallback(std::function<void(const CallbackInfo::BaseCallbackInfo&)> callback) noexcept;

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
