#include "CallbackInfo/ApcMiniInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

std::map<std::uint8_t, std::string> ApcMini::availableColors() noexcept {
	return {{static_cast<std::uint8_t>(Color::None), "None"},
			{static_cast<std::uint8_t>(Color::Yellow), "Yellow"},
			{static_cast<std::uint8_t>(Color::Green), "Green"},
			{static_cast<std::uint8_t>(Color::Red), "Red"}};
}

std::map<std::uint8_t, std::string> ApcMini::availableColorMods() noexcept {
	return {{static_cast<std::uint8_t>(ColorMode::Solid), "Solid"},
			{static_cast<std::uint8_t>(ColorMode::Blink), "Blink"}};
}
