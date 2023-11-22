#include "WorLibrary/DataConverter/DataConverter.hpp"

std::string Wor::DataConverter::sociToString(const soci::row &row, std::int64_t cellIndex) noexcept {
    std::string s {};
    if (row.get_indicator(cellIndex) == soci::i_null) {
        return s;
    }
    const soci::column_properties &property = row.get_properties(cellIndex);
    switch (property.get_data_type()) {
        case soci::dt_double:
            s = std::to_string(row.get<double>(cellIndex));
            break;
        case soci::dt_integer:
            s = std::to_string(row.get<int>(cellIndex));
            break;
        case soci::dt_string:
            s = row.get<std::string>(cellIndex);
            break;
        case soci::dt_long_long:
            s = std::to_string(row.get<long long>(cellIndex));
            break;
        default:
            break;
    }
    return s;
}

[[nodiscard]] std::string toString() noexcept {
    return {};
}
