#pragma once

#include <string>
#include <vector>

namespace Wor::Sql {

    /**
     *
     */
    struct SelectStatementData {

        /**
         *  Value to SELECT scope
         */
        std::vector<std::string> selectValues {};

        /**
         *  Value to FROM scope
         */
        std::string fromValue {};

        /**
         *  Value to ORDER BY scope
         */
        std::string orderByValue {};

        /**
         *  Value to WHERE scope
         */
        std::string whereValue {};

        /**
         * Return sql request via string view with current parameters
         * @return Sql request string
         */
        [[nodiscard]] std::string ToString() const noexcept;
    };
}
