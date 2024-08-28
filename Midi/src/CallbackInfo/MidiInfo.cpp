#include "CallbackInfo/MidiInfo.hpp"

using namespace Wor::Midi::CallbackInfo;

MidiLed::MidiLed() noexcept
	: _color(0),
	  _mode(0) {
}

bool MidiLed::operator==(const MidiLed &rhs) const noexcept {
	return _color == rhs._color && _mode == rhs._mode;
}

#pragma region Accessors/Mutators

std::uint8_t MidiLed::color() const noexcept {
	return _color;
}

std::uint8_t MidiLed::mode() const noexcept {
	return _mode;
}

void MidiLed::color(std::uint8_t color) noexcept {
	_color = color;
}

void MidiLed::mode(std::uint8_t mode) noexcept {
	_mode = mode;
}

Wor::Math::Vector::Vector3<int> MidiLed::rgb() const noexcept {
	return {};
}

std::string MidiLed::colorName() const noexcept {
	return {};
}

std::string MidiLed::modeName() const noexcept {
	return {};
}

#pragma endregion Accessors/Mutators
