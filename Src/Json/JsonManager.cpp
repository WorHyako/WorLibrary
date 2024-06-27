#include "WorLibrary/Json/JsonManager.hpp"

#include <sys/stat.h>

using namespace Wor::Json;

JsonManager::JsonManager()
        : _fileStatus(FileStatus::ZeroCheck),
          _filePath("none") {
}

JsonManager::FileStatus JsonManager::tryToFindFile(const std::string&& filePath, bool create) noexcept {
    if (create) {
        _fileStatus = createFile(filePath)
                      ? FileStatus::Ready
                      : FileStatus::Unreachable;
    } else {
        _fileStatus = isFileExist(filePath)
                      ? FileStatus::Ready
                      : FileStatus::Unreachable;
    }
    if (_fileStatus == FileStatus::Ready) {
        _filePath = filePath;
    }
    return _fileStatus;
}

bool JsonManager::isFileExist(const std::string& filePath) noexcept {
    struct stat buffer{};
    return stat(filePath.c_str(), &buffer) == 0;
}

bool JsonManager::tryToSaveFile(const std::string& jsonString, const std::string& scope) noexcept {
    if (!isFileReady()) {
        return false;
    }
    nlohmann::json fileContent;
    {
        std::ifstream file(_filePath);
        auto fileEmpty = file.peek() == std::ifstream::traits_type::eof();
        if (file.is_open() && !fileEmpty) {
            try {
                fileContent = nlohmann::json::parse(file);
            }
            catch (...) {
                /// Nothing happened. Just forgive this scope.
            }
            if (!fileContent.is_object()) {
                fileContent.clear();
            }
        }
        file.close();
    }
    if (scope.empty()) {
        fileContent.push_back(nlohmann::json::parse(jsonString));
    } else {
        fileContent[scope] = nlohmann::json::parse(jsonString);
    }
    {
        std::ofstream file(_filePath);
        file << std::setw(4) << fileContent.dump(4);
        file.close();
    }
    return true;
}

nlohmann::json JsonManager::tryToLoadFile(const std::string& scopeName) noexcept {
    if (!isFileReady()) {
        return false;
    }
    nlohmann::json fileContent;
    std::ifstream file(_filePath);
    auto fileEmpty = file.peek() == std::ifstream::traits_type::eof();
    if (file.is_open() && !fileEmpty) {
        try {
            fileContent = nlohmann::json::parse(file);
        }
        catch (...) {
            /// Nothing happened. Just forgive this scope.
        }
    }
    file.close();
    return scopeName.empty()
           ? fileContent
           : fileContent[scopeName];
}

inline bool JsonManager::isFileReady() const noexcept {
    return _fileStatus == FileStatus::Ready;
}

bool JsonManager::createFile(const std::string& filePath) noexcept {
    std::ofstream file;
    file.open(filePath, std::ios::app | std::ios::in);
    file.close();
    return isFileExist(filePath);
}

#pragma region Accessors

std::string JsonManager::getFileName() const noexcept {
    return _filePath;
}

JsonManager::FileStatus JsonManager::getFileStatus() const noexcept {
    return _fileStatus;
}

#pragma endregion Accessors
