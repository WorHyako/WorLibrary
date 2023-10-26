#include "WorLibrary/Sql/DataBaseParameters.hpp"

using namespace Wor::Sql;

DataBaseParameters::DataBaseParameters(const std::string &connectionString) noexcept {
    if (connectionString.empty()) {
        return;
    }

    auto getParameter = [](const std::string &str, std::string &&parameterName) {
        auto parameterKeyIndex = str.find(parameterName);
        if (parameterKeyIndex == std::string::npos) {
            return std::string {};
        }
        /*
         * Ignoring '=' symbol
         */
        parameterKeyIndex += 1;
        auto parameterNextKeyIndex = str.find(' ', parameterKeyIndex);
        if (parameterNextKeyIndex == std::string::npos) {
            parameterNextKeyIndex = str.find(';', parameterKeyIndex);
        }
        if (parameterKeyIndex == std::string::npos) {
            return std::string {};
        }
        const auto parameterValueIndex = parameterKeyIndex + parameterName.length();
        return str.substr(parameterValueIndex, parameterNextKeyIndex - parameterValueIndex);
    };

    user = getParameter(connectionString, "user");
    password = getParameter(connectionString, "password");
    host = getParameter(connectionString, "host");
    dbName = getParameter(connectionString, "dbName");
    dbType = getParameter(connectionString, "dbType");
    port = std::stoi(getParameter(connectionString, "port"));
}
