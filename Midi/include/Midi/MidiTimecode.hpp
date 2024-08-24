#pragma once

#include "RtMidi.h"

namespace Wor::Midi {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class MidiTimecode {
		public:
		/**
		 * @brief Ctor.
		 */
		MidiTimecode() noexcept;

		/**
		 * @brief Dtor.
		 */
		virtual ~MidiTimecode() noexcept = default;
	};
}
