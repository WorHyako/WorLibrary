#pragma once

#include <string>
#include <any>

namespace Wor::Sql {

    /**
     *
     */
    struct DbCellView final {
        /**
         * Ctor
         */
        DbCellView() noexcept
                : name({}),
                  value({}) {
        }

        /**
         *
         * @param name
         * @param value
         */
        DbCellView(std::string cellName, std::any cellValue) noexcept
                : name(std::move(cellName)),
                  value(std::move(cellValue)) {
        };

        /**
         *
         */
        std::string name;

        /**
         *
         */
        std::any value;
    };
}
