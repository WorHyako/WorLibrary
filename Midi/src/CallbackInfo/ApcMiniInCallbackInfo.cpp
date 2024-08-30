#include "CallbackInfo/ApcMiniInCallbackInfo.hpp"

#include <CallbackInfo/ApcMiniLed.hpp>

using namespace Wor::Midi::CallbackInfo;

ApcMiniInCallbackInfo::ApcMiniInCallbackInfo(std::vector<std::byte> message) noexcept
	: BaseCallbackInfo(std::move(message)) {
	_buttonIdIdx = 1;
}

#pragma region Accessors/Mutators

ApcMini::KeyEventType ApcMiniInCallbackInfo::type() const noexcept {
	return valid() ? static_cast<ApcMini::KeyEventType>(_message[0]) : static_cast<ApcMini::KeyEventType>(0);
}

std::uint8_t ApcMiniInCallbackInfo::value() const noexcept {
	return valid() ? static_cast<std::uint8_t>(_message[2]) : -1;
}

bool ApcMiniInCallbackInfo::valid() const noexcept {
	return _message.size() == 3;
}

#pragma endregion Accessors/Mutators
