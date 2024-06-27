#pragma once

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

namespace Wor::Json {

    /**
     * @brief
     *
     * @author WorHyako
     */
    class JsonManager final {
    public:
        /**
         * @brief Enum describes current file state
         *
         * @author WorHyako
         */
        enum class FileStatus
                : std::uint8_t {
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
         * @brief   Check file to destination and accessible file.
         *          <p/>
         *          Result will wrote to internal variable.
         *          <p/>
         *          Also file path will saved to internal variable if successful
         *
         * @param   filePath Absolute or relative file path
         *
         * @return  file status
         */
        [[nodiscard]]
        FileStatus tryToFindFile(const std::string &&filePath, bool createFile = true) noexcept;

        /**
         * @brief
         *
         * @param jsonString
         * @param scope
         *
         * @return
         */
        [[nodiscard]] bool
        tryToSaveFile(const std::string &jsonString, const std::string &scope = {}) noexcept;

        /**
         * @brief
         *
         * @param scopeName
         *
         * @return
         */
        [[nodiscard]]
        nlohmann::json tryToLoadFile(const std::string &scopeName = {}) noexcept;

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
         * @brief   Just check file to existing
         *
         * @param   filePath absolute or relative file path
         *
         * @return  checking result
         */
        [[nodiscard]]
        static bool isFileExist(const std::string &filePath) noexcept;

        /**
         * @brief   Small method just to make code more comfortable
         *
         * @return  is file ready
         */
        [[nodiscard]]
        inline bool isFileReady() const noexcept;

        /**
         * @brief
         *
         * @param   filePath
         *
         * @return
         */
        [[nodiscard]]
        static inline bool createFile(const std::string &filePath) noexcept;

    public:
#pragma region Accessors

        [[nodiscard]]
        std::string getFileName() const noexcept;

        [[nodiscard]]
        FileStatus getFileStatus() const noexcept;

#pragma endregion Accessors
    };
}
