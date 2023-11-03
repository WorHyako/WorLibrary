#pragma once

#include "soci/soci.h"

#include <sstream>
#include <string>
#include <any>

namespace Wor::Sql::Utils {

    /**
     *
     */
    class DataConverter final {
    public:

        /**
         *
         * @tparam T
         * @param row
         * @param cellIndex
         * @return
         */
        [[nodiscard]] static std::string SociToString(const soci::row &row, std::int64_t cellIndex) noexcept;

        template<typename Type>
        [[nodiscard]] static std::optional<Type> SociTo(const soci::row &row, std::int64_t cellIndex) noexcept;
    };

    std::string Wor::Sql::Utils::DataConverter::SociToString(const soci::row &row, std::int64_t cellIndex) noexcept {
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

    template<typename Type>
    std::optional<Type> Wor::Sql::Utils::DataConverter::SociTo(const soci::row &row, std::int64_t cellIndex) noexcept {
        std::any value;
        if (row.get_indicator(cellIndex) == soci::i_null) {
            return {};
        }
        const soci::column_properties &property = row.get_properties(cellIndex);
        switch (property.get_data_type()) {
            case soci::dt_double:
                value = row.get<double>(cellIndex);
                break;
            case soci::dt_integer:
                value = row.get<int>(cellIndex);
                break;
            case soci::dt_string:
                value = row.get<std::string>(cellIndex);
                break;
            case soci::dt_long_long:
                value = row.get<long long>(cellIndex);
                break;
            default:
                break;
        }
        try {
            return std::any_cast<Type>(value);
        } catch (const std::bad_any_cast &e) {
            return {};
        }
    }
}
