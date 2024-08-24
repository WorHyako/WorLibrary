#include "CallbackInfo/AkaiApcOutCallbackInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

AkaiApcOutCallbackInfo::AkaiApcOutCallbackInfo(std::byte buttonId,
											   Color color,
											   ColorType colorType) noexcept
	: _messageId(static_cast<std::byte>(0x90)),
	  _color(static_cast<std::byte>(
		  static_cast<std::uint8_t>(color) + static_cast<std::uint8_t>(colorType))),
	  _buttonId(buttonId) {
}

std::vector<std::byte> AkaiApcOutCallbackInfo::message() const noexcept {
	std::vector<std::byte> message(3);
	message.emplace_back(static_cast<std::byte>(_messageId));
	message.emplace_back(static_cast<std::byte>(_buttonId));
	message.emplace_back(static_cast<std::byte>(_color));
	return message;
}
