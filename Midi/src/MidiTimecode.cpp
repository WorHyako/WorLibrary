#include "Midi/MidiTimecode.hpp"

using namespace Wor::Midi;

MidiTimecode::MidiTimecode() noexcept
	: _callback(nullptr) {
}

void MidiTimecode::open(std::uint16_t port, const std::string &portName) noexcept {
}

void MidiTimecode::close() noexcept {
}

#pragma region Accessors/Mutators

bool MidiTimecode::isOpen() const noexcept {
	return _device.isPortOpen();
}

void MidiTimecode::callback(std::function<void(const CallbackInfo::BaseCallbackInfo &)> callback) noexcept {
	_callback = std::move(callback);
}

#pragma endregion Accessors/Mutators
