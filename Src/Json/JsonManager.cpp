#include "Json/JsonManager.hpp"

#include <sys/stat.h>

using namespace Wor::Json;

JsonManager::JsonManager()
        : _fileStatus(FileStatus::ZeroCheck),
          _filePath("none") {
}

JsonManager::FileStatus JsonManager::TryToFindFile(const std::string&& filePath_, bool createFile_) noexcept {
    if (createFile_) {
        _fileStatus = CreateFile(filePath_)
                      ? FileStatus::Ready
                      : FileStatus::Unreachable;
    } else {
        _fileStatus = IsFileExist(filePath_)
                      ? FileStatus::Ready
                      : FileStatus::Unreachable;
    }
    if (_fileStatus == FileStatus::Ready) {
        _filePath = filePath_;
    }
    return _fileStatus;
}

bool JsonManager::IsFileExist(const std::string& filePath_) noexcept {
    struct stat buffer{};
    return stat(filePath_.c_str(), &buffer) == 0;
}

bool JsonManager::TryToSaveFile(const std::string& jsonString_, const std::string& scope_) noexcept {
    if (!IsFileReady()) {
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

nlohmann::json JsonManager::TryToLoadFile(const std::string& scopeName_) noexcept {
    if (!IsFileReady()) {
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

inline bool JsonManager::IsFileReady() const noexcept {
    return _fileStatus == FileStatus::Ready;
}

bool JsonManager::CreateFile(const std::string& filePath_) noexcept {
    std::ofstream file;
    file.open(filePath_, std::ios::app | std::ios::in);
    file.close();
    return IsFileExist(filePath_);
}

#pragma region Accessors

std::string JsonManager::GetFileName() const noexcept {
    return _filePath;
}

JsonManager::FileStatus JsonManager::GetFileStatus() const noexcept {
    return _fileStatus;
}

#pragma endregion Accessors
