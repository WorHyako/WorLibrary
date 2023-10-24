#include "WorLibrary/Sql/MySqlManager.hpp"

#include <iostream>
#include <utility>

using namespace Wor::Sql;

MySqlManager::MySqlManager(DataBaseParameters dbParameters) noexcept :
        _status(ConnectionStatus::ZeroCheck),
        _dbParameters(std::move(dbParameters)) {
}

MySqlManager::ConnectionStatus MySqlManager::TryToConnect() noexcept {
    const std::string connectionString {
            "db=" + _dbParameters.dbName +
            " user=" + _dbParameters.user +
            " password=" + _dbParameters.password +
            " host=" + _dbParameters.host +
            " port=" + std::to_string(_dbParameters.port) };
    try {
        soci::session sql(soci::mysql, connectionString);
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