#include "Midi/MidiDeviceList.hpp"

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
	for (int i = 0; i < portCount; i++) {
		deviceList.emplace_back(midiOut.getPortName(i));
	}
	return deviceList;
}

std::vector<std::string> MidiDeviceList::getKeyboards() noexcept {
	std::vector<std::string> deviceList;
	auto inDevices = getIn();
	auto outDevices = getOut();
	deviceList.reserve(outDevices.size() + inDevices.size());
	std::ranges::for_each(
			inDevices,
			[&outDevices, &deviceList](const auto &in) {
				if (std::find(std::begin(outDevices),
							  std::end(outDevices),
							  in) != std::end(
						outDevices)) {
					deviceList.emplace_back(in);
				}
			});
	return deviceList;
}
