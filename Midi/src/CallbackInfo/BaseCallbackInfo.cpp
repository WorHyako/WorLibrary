#include "CallbackInfo/BaseCallbackInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

BaseCallbackInfo::BaseCallbackInfo(std::vector<std::byte> message) noexcept
	: _message(std::move(message)) {
}

#pragma region Accessors/Mutators

std::vector<unsigned char> BaseCallbackInfo::vector() const noexcept {
	std::vector<unsigned char> result;
	std::for_each(std::begin(_message),
				  std::end(_message),
				  [&result](std::byte byte) {
					  result.emplace_back(static_cast<unsigned char>(byte));
				  });
	return result;
}

bool BaseCallbackInfo::valid() const noexcept {
	return !_message.empty();
}

#pragma endregion Accessors/Mutators
