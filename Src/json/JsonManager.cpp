#include "Json/JsonManager.hpp"

#include <sys/stat.h>

using namespace wor::Json;

JsonManager::JsonManager()
        : fileStatus(FileStatus::ZeroCheck),
          _filePath("none") {
}

JsonManager::FileStatus JsonManager::TryToFindFile(const std::string&& filePath_) noexcept {
    fileStatus = IsFileExist(filePath_)
                 ? FileStatus::Ready
                 : FileStatus::Unreachable;
    if (fileStatus == FileStatus::Ready) {
        _filePath = filePath_;
    }
    return fileStatus;
}

bool JsonManager::IsFileExist(const std::string& filePath_) noexcept {
    struct stat buffer{};
    return (stat(filePath_.c_str(), &buffer) == 0);
}

bool JsonManager::TryToSaveFile(const std::string& configString) noexcept {
    if (!IsFileReady()) {
        return false;
    }
    nlohmann::json nlJson(configString);
    return true;
}

bool JsonManager::TryToLoadFile() noexcept {
    if (!IsFileReady()) {
        return false;
    }
    return true;
}

inline bool JsonManager::IsFileReady() const noexcept {
    return fileStatus == FileStatus::Ready;
}

#pragma region Accessors

std::string JsonManager::GetFileName() const noexcept {
    return _filePath;
}

#pragma endregion Accessors
