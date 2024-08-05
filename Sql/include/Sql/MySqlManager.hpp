#pragma once

#include "soci/soci.h"
#include "soci/empty/soci-empty.h"
#include "soci/mysql/soci-mysql.h"

#include "Sql/DataBaseParameters.hpp"
#include "Sql/Event/SelectStatementData.hpp"
#include "Sql/DbTableView.hpp"

#include <string>

namespace Wor::Sql {

    /**
     * @brief
     *
     * @author WorHyako
     */
    class MySqlManager {
    public:
        /**
         * @brief
         *
         * @author WorHyako
         */
        enum class ConnectionStatus
                : std::uint8_t {
            /**
             *
             */
            ZeroCheck,

            /**
             *
             */
            Connected,

            /**
             *
             */
            AuthFailed,

            /**
             *
             */
            Unreachable
        };

        /**
         * @brief
         *
         * @param dbParameters
         */
        void configure(DataBaseParameters dbParameters) noexcept;

        /**
         * @brief
         *
         * @param connectionString
         *
         * @return
         */
        [[nodiscard]]
        ConnectionStatus tryToConnect() noexcept;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        DbTableView select(const Event::SelectStatementData &statementData) noexcept;

    protected:
        /**
         * @brief Ctor.
         *
         * @param dbParameters
         */
        explicit MySqlManager(DataBaseParameters dbParameters = {}) noexcept;

    private:
        /**
         * Manager current status
         */
        ConnectionStatus _status;

        /**
         * Current data base session
         */
        soci::session _session;

        /**
         * Data Base parameters for connection string
         */
        DataBaseParameters _dbParameters;

    public:
#pragma region Accessors

        [[nodiscard]]
        ConnectionStatus status() const noexcept;

        [[nodiscard]]
        const DataBaseParameters &dpParameters() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

        [[nodiscard]]
        bool dpParameters(DataBaseParameters dpParameters) noexcept;

#pragma endregion Mutators
    };
}
