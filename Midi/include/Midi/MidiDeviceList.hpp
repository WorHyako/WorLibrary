#pragma once

#include <vector>

namespace Wor::Midi::MidiDeviceList {
	/**
	 * @brief
	 *
	 * @return
	 *
	 * @author WorHyako
	 */
	[[nodiscard]]
	std::vector<std::string> getIn() noexcept;

	/**
	 * @brief
	 *
	 * @return
	 *
	 * @author WorHyako
	 */
	[[nodiscard]]
	std::vector<std::string> getOut() noexcept;

	/**
	 *
	 * @return
	 *
	 * @author WorHyako
	 */
	[[nodiscard]]
	std::vector<std::string> getKeyboards() noexcept;
}
