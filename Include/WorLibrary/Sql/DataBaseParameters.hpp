#pragma once

#include <string>

namespace Wor::Sql {

    /**
     * @brief Provide data to connect to Data Base
     *
     * @author WorHyako
     */
    struct DataBaseParameters final {
        /**
         * @brief Ctor.
         *
         * @param connectionString
         */
        explicit DataBaseParameters(const std::string &connectionString) noexcept;

        /**
         * @brief Ctor.
         */
        DataBaseParameters() noexcept = default;

        /**
         * Data base type
         */
        std::string dbType = "mysql";

        /**
         * Data base name
         */
        std::string dbName = "test";

        /**
         * User name for data base
         */
        std::string user = "user";

        /**
         * User password for data base
         */
        std::string password = "password";

        /**
         * Data base location ip
         */
        std::string host = "127.0.0.1";

        /**
         * Data base location socket port
         */
        uint16_t port = -1;
    };
}
