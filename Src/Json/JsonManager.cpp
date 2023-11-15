#include "WorLibrary/Json/JsonManager.hpp"

#include <sys/stat.h>

using namespace Wor::Json;

JsonManager::JsonManager()
        : _fileStatus(FileStatus::ZeroCheck),
          _filePath("none") {
}

JsonManager::FileStatus JsonManager::tryToFindFile(const std::string&& filePath_, bool createFile_) noexcept {
    if (createFile_) {
        _fileStatus = createFile(filePath_)
                      ? FileStatus::Ready
                      : FileStatus::Unreachable;
    } else {
        _fileStatus = isFileExist(filePath_)
                      ? FileStatus::Ready
                      : FileStatus::Unreachable;
    }
    if (_fileStatus == FileStatus::Ready) {
        _filePath = filePath_;
    }
    return _fileStatus;
}

bool JsonManager::isFileExist(const std::string& filePath_) noexcept {
    struct stat buffer{};
    return stat(filePath_.c_str(), &buffer) == 0;
}

bool JsonManager::tryToSaveFile(const std::string& jsonString_, const std::string& scope_) noexcept {
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
    if (scope_.empty()) {
        fileContent.push_back(nlohmann::json::parse(jsonString_));
    } else {
        fileContent[scope_] = nlohmann::json::parse(jsonString_);
    }
    {
        std::ofstream file(_filePath);
        file << std::setw(4) << fileContent.dump(4);
        file.close();
    }
    return true;
}

nlohmann::json JsonManager::tryToLoadFile(const std::string& scopeName_) noexcept {
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
    return scopeName_.empty()
           ? fileContent
           : fileContent[scopeName_];
}

inline bool JsonManager::isFileReady() const noexcept {
    return _fileStatus == FileStatus::Ready;
}

bool JsonManager::createFile(const std::string& filePath_) noexcept {
    std::ofstream file;
    file.open(filePath_, std::ios::app | std::ios::in);
    file.close();
    return isFileExist(filePath_);
}

#pragma region Accessors

std::string JsonManager::getFileName() const noexcept {
    return _filePath;
}

JsonManager::FileStatus JsonManager::getFileStatus() const noexcept {
    return _fileStatus;
}

#pragma endregion Accessors
