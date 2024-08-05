#pragma once

#include <string>

#include "nlohmann/json.hpp"

namespace Wor::Json {

    /**
     * @brief
     *
     * @param filePath
     *
     * @param jsonString
     *
     * @return
     *
     * @author WorHyako
     */
    [[nodiscard]]
    bool tryToSaveFile(const std::string &filePath, const std::string &jsonString) noexcept;

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
    nlohmann::json tryToLoadFile(const std::string &filePath) noexcept;
}
