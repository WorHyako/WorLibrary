#pragma once

#include <sstream>
#include <string>
#include <any>

#ifndef SOCI_H_INCLUDED

#include "soci/soci.h"

#endif

namespace Wor::DataConverter {

#ifdef SOCI_H_INCLUDED

    /**
     *
     * @param row
     * @param cellIndex
     * @return
     */
    [[nodiscard]]
    std::string sociToString(const soci::row &row, std::int64_t cellIndex) noexcept;

    /**
     *
     * @tparam Type
     * @param row
     * @param cellIndex
     * @return
     */
    template<typename Type>
    [[nodiscard]]
    std::optional<Type> sociTo(const soci::row &row, std::int64_t cellIndex) noexcept {
        if (row.get_indicator(cellIndex) == soci::i_null) {
            return {};
        }
        const soci::column_properties &property = row.get_properties(cellIndex);
        std::any value;
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

#endif
}
