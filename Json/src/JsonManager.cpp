#include "Json/JsonManager.hpp"

#include <fstream>
#include <sstream>

#include <spdlog/spdlog.h>

using namespace Wor;

namespace {
	/**
	 * @brief
	 *
	 * @param filePath
	 *
	 * @return
	 *
	 * @author WorHyako
	 */
	[[nodiscard]]
	bool checkFile(const std::string &filePath) noexcept {
		std::ofstream file;
		try {
			file.open(filePath, std::ios::app | std::ios::in);
			file.close();
		} catch (const std::ofstream::failure &e) {
			std::stringstream ss;
			ss << "Error opening file "
					<< filePath
					<< ": "
					<< e.what();
			spdlog::error(ss.str());
			return false;
		}
		return true;
	}
}

bool Json::tryToSaveFile(const std::string &filePath, const std::string &jsonString) noexcept {
	if (!checkFile(filePath)) {
		return false;
	}
	nlohmann::json fileContent;
	{
		std::ifstream file(filePath);
		auto fileEmpty = file.peek() == std::ifstream::traits_type::eof();
		if (file.is_open() && !fileEmpty) {
			try {
				fileContent = nlohmann::json::parse(file);
			} catch (const nlohmann::json::exception &e) {
				std::stringstream ss;
				ss << "Failed to parse "
						<< filePath
						<< " file: "
						<< e.what();
				spdlog::error(ss.str());
			}
			if (!fileContent.is_object()) {
				fileContent.clear();
			}
		}
		file.close();
	}
	std::ofstream file(filePath);
	if (!file.is_open()) {
		return false;
	}
	fileContent = nlohmann::json::parse(jsonString);
	file << std::setw(4) << fileContent.dump(4);
	file.close();
	return true;
}

nlohmann::json Json::tryToLoadFile(const std::string &filePath) noexcept {
	if (!checkFile(filePath)) {
		return false;
	}
	nlohmann::json fileContent;
	std::ifstream file(filePath);
	const auto fileEmpty = file.peek() == std::ifstream::traits_type::eof();
	if (!file.is_open() && fileEmpty) {
		return {};
	}
	try {
		fileContent = nlohmann::json::parse(file);
	} catch (const nlohmann::json::exception &e) {
		std::stringstream ss;
		ss << "Failed to parse "
				<< filePath
				<< " file\nError info:"
				<< e.what();
		spdlog::error(ss.str());
	}

	file.close();
	return fileContent;
}
