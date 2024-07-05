#include "WorLibrary/Json/JsonManager.hpp"

#include <fstream>

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
            std::printf("Exception opening/reading file\n");
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
            }
            catch (const nlohmann::json::exception& e) {
                std::printf("Fail to parse %s file\n Error: %s", filePath.c_str(), e.what());
            }
            if (!fileContent.is_object()) {
                fileContent.clear();
            }
        }
        file.close();
    }
    fileContent.push_back(nlohmann::json::parse(jsonString));
    {
        std::ofstream file(filePath);
        file << std::setw(4) << fileContent.dump(4);
        file.close();
    }
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
    }
    catch (...) {
        std::printf("Fail to parse %s file\n", filePath.c_str());
    }

    file.close();
    return fileContent;
}
