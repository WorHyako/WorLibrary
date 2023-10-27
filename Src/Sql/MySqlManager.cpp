#include "WorLibrary/Sql/MySqlManager.hpp"

#include <iostream>
#include <utility>

#include "WorLibrary/Sql/Utils/DataConverter.hpp"

using namespace Wor::Sql;

namespace {

    /**
     * Check if rowset is empty
     */
    bool CheckEmptiness(const soci::rowset<soci::row> &rowset) {
        return rowset.begin() == rowset.end();
    }
}

MySqlManager::MySqlManager(DataBaseParameters dbParameters) noexcept :
        _status(ConnectionStatus::ZeroCheck) {
    Configure(std::move(dbParameters));
}

void MySqlManager::Configure(DataBaseParameters dbParameters) noexcept {
    _dbParameters = std::move(dbParameters);
}

MySqlManager::ConnectionStatus MySqlManager::TryToConnect() noexcept {
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
    return Status();
}

TableMap<std::string> MySqlManager::Select(const SelectStatementData &statementData) noexcept {
    soci::rowset<soci::row> rs = _session.prepare << statementData.ToString();
    if (!CheckEmptiness(rs)) {
        return {};
    }
    TableMap<std::string> tableMap;
    uint16_t rowCount = 0;
    for (const auto &row : rs) {
        std::vector<DbCellStrView<std::string>> tableRow;
        for (std::size_t i = 0; i < statementData.selectValues.size(); i++) {
            const auto strValue = Utils::DataConverter::SociToString(row, i);
            tableRow.emplace_back(statementData.selectValues[i], strValue);
        }
        tableMap.emplace_back(tableRow);
        rowCount++;
    }
    return tableMap;
}

#pragma region Accessors

MySqlManager::ConnectionStatus MySqlManager::Status() const noexcept {
    return _status;
}

const DataBaseParameters &MySqlManager::DpParameters() const noexcept {
    return _dbParameters;
}

#pragma endregion Accessors

#pragma region Mutators

bool MySqlManager::DpParameters(DataBaseParameters dpParameters) noexcept {
    _dbParameters = std::move(dpParameters);
    return true;
}

#pragma endregion Mutators