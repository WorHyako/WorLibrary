#include "Midi/CallbackInfo/BaseCallbackInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

BaseCallbackInfo::BaseCallbackInfo(std::vector<std::byte> message) noexcept
	: _message(std::move(message)),
	  _buttonIdIdx(0) {
}

#pragma region Accessors/Mutators

std::uint8_t BaseCallbackInfo::buttonId() const noexcept {
	return static_cast<std::uint8_t>(_message[_buttonIdIdx]);
}

std::vector<unsigned char> BaseCallbackInfo::vector() const noexcept {
	std::vector<unsigned char> result;
	std::ranges::for_each(_message,
						  [&result](std::byte byte) {
							  result.emplace_back(static_cast<unsigned char>(byte));
						  });
	return result;
}

bool BaseCallbackInfo::valid() const noexcept {
	return !_message.empty();
}

#pragma endregion Accessors/Mutators
