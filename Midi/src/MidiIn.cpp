#include "MidiIn.hpp"

#include "RtMidi.h"

using namespace Wor::Midi;

void MidiIn::test() {
	RtMidiIn t;
	t.openPort();
	auto open = t.isPortOpen();
	auto name = t.getPortName();
	auto api = t.getCurrentApi();
	auto apiName = RtMidiIn::getApiName(api);
}
