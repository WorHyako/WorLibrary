#include "MidiKeyboard.hpp"

using namespace Wor::Midi;

MidiKeyboard::MidiKeyboard() noexcept
	: _callback(nullptr) {
}

void MidiKeyboard::open(std::uint16_t port, const std::string &portName) noexcept {
	try {
		_inDevice.openPort(port, portName);
		_outDevice.openPort(port, portName);
	} catch (const RtMidiError &e) {
		std::printf("MidiKeyboard::open:\n\tName:port: %s%i\n\tError: %s\n",
					portName.c_str(),
					port,
					e.what());
	}
	_inDevice.setCallback(&MidiKeyboard::nativeInCallback, this);
	_inDevice.setErrorCallback(&MidiKeyboard::errorInCallback, this);
	_outDevice.setErrorCallback(&MidiKeyboard::errorOutCallback, this);
}

void MidiKeyboard::close() noexcept {
	if (isOpen()) {
		_inDevice.closePort();
		_outDevice.closePort();
	}
}

void MidiKeyboard::send(const CallbackInfo::BaseCallbackInfo &callbackInfo) noexcept {
	if (!isOpen()) {
		return;
	}

	const auto &message = callbackInfo.vector();
	_outDevice.sendMessage(&message);
}

void MidiKeyboard::nativeInCallback(double timeStamp, std::vector<unsigned char> *message, void *user) noexcept {
	auto userClass = static_cast<MidiKeyboard *>(user);
	if (userClass == nullptr
		|| message == nullptr
		|| message->size() != 3
		|| userClass->_callback == nullptr) {
		return;
	}
	std::vector<std::byte> byteMessage;
	std::ranges::for_each(*message,
						  [&byteMessage](unsigned char each) {
							  byteMessage.emplace_back(static_cast<std::byte>(each));
						  });
	CallbackInfo::BaseCallbackInfo callbackInfo(byteMessage);
	/**
	 * TODO: make async
	 */
	userClass->_callback(callbackInfo);
}

void MidiKeyboard::errorInCallback(RtMidiError::Type error, const std::string &errorText, void *user) noexcept {
	auto userClass = static_cast<MidiKeyboard *>(user);
	if (userClass == nullptr || errorText.empty()) {
		return;
	}
	std::printf("\n\tError text: %s\n\tError: %i\n", errorText.c_str(), error);
}

void MidiKeyboard::errorOutCallback(RtMidiError::Type error, const std::string &errorText, void *user) noexcept {
}

#pragma region Accessors/Mutators

bool MidiKeyboard::isOpen() const noexcept {
	return _inDevice.isPortOpen() && _outDevice.isPortOpen();
}

void MidiKeyboard::inCallback(std::function<void(const CallbackInfo::BaseCallbackInfo &)> callback) noexcept {
	_callback = std::move(callback);
}

void MidiKeyboard::errorCallback(RtMidiErrorCallback callback) noexcept {
	_inDevice.setErrorCallback(callback);
}

#pragma endregion Accessors/Mutators
