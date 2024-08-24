#include "MidiDeviceList.hpp"

#include <RtMidi.h>

using namespace Wor::Midi;

std::vector<std::string> MidiDeviceList::getIn() noexcept {
	RtMidiIn midiIn;
	auto portCount = midiIn.getPortCount();

	std::vector<std::string> deviceList;
	deviceList.reserve(portCount);
	for (int i = 0; i < portCount; i++) {
		deviceList.emplace_back(midiIn.getPortName(i));
	}
	return deviceList;
}

std::vector<std::string> MidiDeviceList::getOut() noexcept {
	RtMidiOut midiOut;
	auto portCount = midiOut.getPortCount();

	std::vector<std::string> deviceList;
	deviceList.reserve(portCount);
	deviceList.resize(portCount);
	for (int i = 0; i < portCount; i++) {
		deviceList.emplace_back(midiOut.getPortName(i));
	}
	return deviceList;
}

std::vector<std::string> MidiDeviceList::getKeyboards() noexcept {
	std::vector<std::string> deviceList;
	auto inDevices = getIn();
	auto outDevices = getOut();
	for (auto& in : inDevices) {
		if(std::find(std::begin(outDevices), std::begin(outDevices), in) != std::end(outDevices)) {
			deviceList.emplace_back(in);
		}
	}
	return deviceList;
}
