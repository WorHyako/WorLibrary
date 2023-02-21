#ifndef MEDIASERVERMANAGER_WOR_JSON_JSONMANAGER_HPP
#define MEDIASERVERMANAGER_WOR_JSON_JSONMANAGER_HPP

#include <fstream>
#include <iostream>

#include "../ThirdParty/nlohmann/json/single_include/json.hpp"

namespace wor::Json {

    class JsonManager {
    public:
        enum class FileStatus {
            ZeroCheck,
            Unreachable,
            Ready
        } _fileStatus;

        JsonManager();

        virtual ~JsonManager() = default;

        [[nodiscard]] FileStatus static TryToFindFile(const std::string&& filePath_) noexcept;

    private:
        [[nodiscard]] static bool IsFileExist(const std::string& filePath_) noexcept;

    public:
#pragma region Accessors
#pragma endregion Accessors

#pragma region Mutators
#pragma endregion Mutators
    };
}
#endif
