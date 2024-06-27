#pragma once

#include <string>

namespace Wor::Sql {

    /**
     * @brief
     *
     * @author WorHyako
     */
    struct DbCellView final {
        /**
         * @brief Ctor
         */
        DbCellView() noexcept
                : _name({}),
                  _value({}) {
        }

        /**
         * @brief   Ctor.
         *
         * @param   name
         * @param   value
         */
        DbCellView(std::string cellName, std::string cellValue) noexcept
                : _name(std::move(cellName)),
                  _value(std::move(cellValue)) {
        };

        /**
         *
         */
        std::string _name;

        /**
         *
         */
        std::string _value;
    };
}
