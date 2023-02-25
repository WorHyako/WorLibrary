#include "Json/JsonManager.hpp"

#include <sys/stat.h>

using namespace wor::Json;

JsonManager::JsonManager()
        : _fileStatus(FileStatus::ZeroCheck),
          _filePath("none") {
}

JsonManager::FileStatus JsonManager::TryToFindFile(const std::string&& filePath_, bool createFile_) noexcept {
    _fileStatus = IsFileExist(filePath_)
                  ? FileStatus::Ready
                  : FileStatus::Unreachable;
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
        fileContent["QuickButtonsScope"] = nlohmann::json::parse(jsonString_);
    }
    {
        std::ofstream file(_filePath);
        file << std::setw(4) << fileContent.dump(4);
        file.close();
    }
    return true;
}

bool JsonManager::TryToLoadFile(const std::string& scope_) noexcept {
    if (!IsFileReady()) {
        return false;
    }
    return true;
}

inline bool JsonManager::IsFileReady() const noexcept {
    return _fileStatus == FileStatus::Ready;
}

#pragma region Accessors

std::string JsonManager::GetFileName() const noexcept {
    return _filePath;
}

JsonManager::FileStatus JsonManager::GetFileStatus() const noexcept {
    return _fileStatus;
}

#pragma endregion Accessors
