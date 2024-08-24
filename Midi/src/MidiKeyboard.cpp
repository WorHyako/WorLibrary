#include "MidiKeyboard.hpp"

#include <future>

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
	_inDevice.setCallback([](double timeStamp, std::vector<unsigned char> *message, void *user) {
							  auto userClass = static_cast<MidiKeyboard *>(user);
							  if (userClass == nullptr
								  || message == nullptr
								  || message->size() != 3
								  || userClass->_callback == nullptr) {
								  return;
							  }
							  CallbackInfo::AkaiApcInCallbackInfo callbackInfo(
									  static_cast<std::byte>(message->at(1)),
									  static_cast<CallbackInfo::KeyEventType>(message->at(0)),
									  static_cast<std::byte>(message->at(2)));
							  /**
							   * TODO: make async
							   */
							  userClass->_callback(callbackInfo);
						  },
						  this);
	_inDevice.setErrorCallback([](RtMidiError::Type error, const std::string &errorText, void *user) {
								   auto userClass = static_cast<MidiKeyboard *>(user);
								   if (userClass == nullptr || errorText.empty()) {
									   return;
								   }
								   std::printf("\n\tError text: %s\n\tError: %i\n", errorText.c_str(), error);
							   },
							   this);
}

void MidiKeyboard::close() noexcept {
	if (isOpen()) {
		_inDevice.closePort();
		_outDevice.closePort();
	}
}

void MidiKeyboard::send(int buttonId, int value) noexcept {
	if (!isOpen()) {
		return;
	}
	std::vector<unsigned char> message = {0x90,
										  static_cast<unsigned char>(buttonId),
										  static_cast<unsigned char>(value)};
	_outDevice.sendMessage(&message);
}

#pragma region Accessors/Mutators

bool MidiKeyboard::isOpen() const noexcept {
	return _inDevice.isPortOpen() && _outDevice.isPortOpen();
}

void MidiKeyboard::callback(std::function<void(const CallbackInfo::AkaiApcInCallbackInfo &)> callback) noexcept {
	_callback = std::move(callback);
}

void MidiKeyboard::errorCallback(RtMidiErrorCallback callback) noexcept {
	_inDevice.setErrorCallback(callback);
}

#pragma endregion Accessors/Mutators
