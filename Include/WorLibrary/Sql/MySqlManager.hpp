#pragma once

#include "soci/soci.h"
#include "soci/empty/soci-empty.h"
#include "soci/mysql/soci-mysql.h"

#include "WorLibrary/Sql/DataBaseParameters.hpp"
#include "WorLibrary/Sql/Event/SelectStatementData.hpp"
#include "WorLibrary/Sql/DbTableView.hpp"

#include <string>

namespace Wor::Sql {

    /**
     *
     */
    class MySqlManager {
    public:

        /**
         *
         */
        enum class ConnectionStatus {

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
         *
         * @param dbParameters
         */
        void configure(DataBaseParameters dbParameters) noexcept;

        /**
         *
         * @param connectionString
         * @return
         */
        [[nodiscard]] ConnectionStatus tryToConnect() noexcept;

        /**
         *
         * @return
         */
        [[nodiscard]] DbTableView select(const Event::SelectStatementData &statementData) noexcept;

    protected:

        /**
         * Ctor
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

        [[nodiscard]] ConnectionStatus status() const noexcept;

        [[nodiscard]] const DataBaseParameters &dpParameters() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

        [[nodiscard]] bool dpParameters(DataBaseParameters dpParameters) noexcept;

#pragma endregion Mutators
    };
}
