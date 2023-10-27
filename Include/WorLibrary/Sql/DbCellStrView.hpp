#pragma once

#include <string>

namespace Wor::Sql {

    /**
     *
     */
    template<typename T = std::string>
    struct DbCellStrView final {
        /**
         * Ctor
         */
        DbCellStrView();

        /**
         *
         * @param name
         * @param value
         */
        DbCellStrView(std::string name, std::string value);

        /**
         *
         */
        std::string name;

        /**
         *
         */
        T value;
    };

    /**
     *
     */
    template<typename T = std::string>
    using TableMap = std::vector<std::vector<Wor::Sql::DbCellStrView<T>>>;

    template<typename T>
    DbCellStrView<T>::DbCellStrView(std::string cellName, std::string cellValue)
            : name(std::move(cellName)),
              value(cellValue) {
    }

    template<typename T>
    Wor::Sql::DbCellStrView<T>::DbCellStrView()
            : name({}),
              value(T {}) {
    }
}
