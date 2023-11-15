#include "WorLibrary/Sql/MySqlManager.hpp"

#include <iostream>
#include <utility>

#include "WorLibrary/Sql/Utils/DataConverter.hpp"

using namespace Wor::Sql;

MySqlManager::MySqlManager(DataBaseParameters dbParameters) noexcept :
        _status(ConnectionStatus::ZeroCheck) {
    configure(std::move(dbParameters));
}

void MySqlManager::configure(DataBaseParameters dbParameters) noexcept {
    _dbParameters = std::move(dbParameters);
}

MySqlManager::ConnectionStatus MySqlManager::tryToConnect() noexcept {
    const std::string connectionString {
            "db=" + _dbParameters.dbName +
            " user=" + _dbParameters.user +
            " password=" + _dbParameters.password +
            " host=" + _dbParameters.host +
            " port=" + std::to_string(_dbParameters.port) };
    try {
        _session.open(soci::mysql, connectionString);
        _status = ConnectionStatus::Connected;
    }
    catch (const soci::soci_error &e) {
        std::cerr << "Connection to \"" << connectionString << "\" failed: "
                  << e.what() << "\n";
        _status = ConnectionStatus::AuthFailed;
    }
    catch (...) {
        _status = ConnectionStatus::Unreachable;
    }
    return status();
}

DbTableView MySqlManager::select(const Event::SelectStatementData &statementData) noexcept {
    const soci::rowset<soci::row>& rowset = _session.prepare << statementData.toString();
    DbTableView tableMap;
    for (const auto &row : rowset) {
        DbRowView tableRow;

        const auto eventIdOpt = Utils::DataConverter::sociTo<int>(row, static_cast<std::int64_t>(0));
        if (!eventIdOpt.has_value()) {
            return {};
        }
        for (std::size_t i = 0; i < statementData.selectValues.size(); i++) {
            const auto strValue = Utils::DataConverter::sociToString(row, static_cast<std::int64_t>(i));
            tableRow.add({ statementData.selectValues[i], strValue });
        }

        tableMap.AddRow(tableRow, true);
    }
    return tableMap;
}

#pragma region Accessors

MySqlManager::ConnectionStatus MySqlManager::status() const noexcept {
    return _status;
}

const DataBaseParameters &MySqlManager::dpParameters() const noexcept {
    return _dbParameters;
}

#pragma endregion Accessors

#pragma region Mutators

bool MySqlManager::dpParameters(DataBaseParameters dpParameters) noexcept {
    _dbParameters = std::move(dpParameters);
    return true;
}

#pragma endregion Mutators
