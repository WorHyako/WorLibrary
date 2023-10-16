#pragma once

#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

namespace Wor::Json {

    /**
     *
     */
    class JsonManager final {
    public:
        /**
         * Enum describes current file state
         */
        enum class FileStatus {
            /**
             * File didn't checked
             */
            ZeroCheck,
            /**
             * File don't exist
             */
            Unreachable,
            /**
             * File is busy by other process
             */
            Busy,
            /**
             * File ready to read/write operation
             */
            Ready
        };

        /**
         * Ctor
         */
        JsonManager();

        /**
         * Check file to destination and accessible file\n
         * Result will wrote to internal variable\n
         * Also file path will saved to internal variable if successful
         * @param filePath_ Absolute or relative file path
         * @return          file status
         */
        [[nodiscard]] FileStatus TryToFindFile(const std::string&& filePath_, bool createFile_ = true) noexcept;

        /**
         *
         * @param jsonString_
         * @param scope_
         * @return
         */
        [[nodiscard]] bool
        TryToSaveFile(const std::string& jsonString_, const std::string& scope_ = {}) noexcept;

        /**
         *
         * @param scopeName_
         * @return
         */
        [[nodiscard]] nlohmann::json TryToLoadFile(const std::string& scopeName_ = {}) noexcept;

    private:
        /**
         * Path to current status
         */
        std::string _filePath;

        /**
         * Current file status
         */
        FileStatus _fileStatus;

        /**
         * Just check file to existing
         * @param filePath_ absolute or relative file path
         * @return          checking result
         */
        [[nodiscard]] static bool IsFileExist(const std::string& filePath_) noexcept;

        /**
         * Small method just to make code more comfortable
         * @return  is file ready
         */
        [[nodiscard]] inline bool IsFileReady() const noexcept;

        /**
         *
         * @param filePath_
         * @return
         */
        [[nodiscard]] static inline bool CreateFile(const std::string& filePath_) noexcept;

    public:
#pragma region Accessors

        [[nodiscard]] std::string GetFileName() const noexcept;

        [[nodiscard]] FileStatus GetFileStatus() const noexcept;

#pragma endregion Accessors
    };
}
