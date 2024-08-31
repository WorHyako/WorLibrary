#include "Midi/CallbackInfo/ApcMiniOutCallbackInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

ApcMiniOutCallbackInfo::ApcMiniOutCallbackInfo(uint8_t buttonId, const MidiLed& led) noexcept
	: BaseCallbackInfo({}) {
	_message.resize(3);
	_message[0] = static_cast<std::byte>(0x90);
	_message[1] = static_cast<std::byte>(buttonId);
	_message[2] = static_cast<std::byte>(led.color() + led.mode());
}

#pragma region Accessors/Mutators

bool ApcMiniOutCallbackInfo::valid() const noexcept {
	return _message.size() == 3;
}

#pragma endregion Accessors/Mutators
