#include "CallbackInfo/ApcMiniLed.hpp"

using namespace Wor::Midi::CallbackInfo::ApcMini;
using namespace Wor::Midi::CallbackInfo;

ApcMiniLed::ApcMiniLed() noexcept {
	_color = static_cast<std::uint8_t>(Color::None);
	_mode = static_cast<std::uint8_t>(Mode::Solid);
}

ApcMiniLed::ApcMiniLed(Color color, Mode mode) noexcept {
	_color = static_cast<std::uint8_t>(color);
	_mode = static_cast<std::uint8_t>(mode);
}

#pragma region Accessors/Mutators

std::string ApcMiniLed::colorName() const noexcept {
	std::string colorName;
	switch (static_cast<Color>(_color)) {
		case Color::None:
			colorName = "None";
			break;
		case Color::Green:
			colorName = "Green";
			break;
		case Color::Red:
			colorName = "Red";
			break;
		case Color::Yellow:
			colorName = "Yellow";
			break;
		default:
			colorName = "None";
	}
	return colorName;
}

std::string ApcMiniLed::modeName() const noexcept {
	std::string colorMode;
	switch (static_cast<Mode>(_mode)) {
		case Mode::Solid:
			colorMode = "Solid";
			break;
		case Mode::Blink:
			colorMode = "Blink";
			break;
		default:
			colorMode = "Solid";
	}
	return colorMode;
}

Wor::Math::Vector::Vector3<int> ApcMiniLed::rgb() const noexcept {
	Math::Vector::Vector3<int> rgb;
	switch (static_cast<Color>(_color)) {
		case Color::None:
			rgb = Math::Vector::Vector3<int>(200, 200, 200);
			break;
		case Color::Green:
			rgb = Math::Vector::Vector3<int>(0, 200, 0);
			break;
		case Color::Red:
			rgb = Math::Vector::Vector3<int>(200, 0, 0);
			break;
		case Color::Yellow:
			rgb = Math::Vector::Vector3<int>(255, 79, 0);
			break;
		default:
	}
	return rgb;
}

std::vector<ApcMiniLed> ApcMiniLed::availableLeds() noexcept {
	return {{Color::None, Mode::Solid},
			{Color::Green, Mode::Solid},
			{Color::Green, Mode::Blink},
			{Color::Red, Mode::Solid},
			{Color::Red, Mode::Blink},
			{Color::Yellow, Mode::Solid},
			{Color::Yellow, Mode::Blink}};
}

#pragma endregion Accessors/Mutators
