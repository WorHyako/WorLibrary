#include "CallbackInfo/AkaiApcInCallbackInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

AkaiApcInCallbackInfo::AkaiApcInCallbackInfo(std::byte buttonId, KeyEventType type, std::byte value) noexcept
	: _type(type),
	  _buttonId(buttonId),
	  _value(value) {
}

#pragma region Accessors/Mutators

KeyEventType AkaiApcInCallbackInfo::type() const noexcept {
	return _type;
}

std::byte AkaiApcInCallbackInfo::value() const noexcept {
	return _value;
}

std::byte AkaiApcInCallbackInfo::buttonId() const noexcept {
	return _buttonId;
}

#pragma endregion Accessors/Mutators
