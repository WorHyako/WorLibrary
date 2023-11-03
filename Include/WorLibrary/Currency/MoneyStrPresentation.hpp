#pragma once

#include <string>

namespace Wor::Currency {

    const std::string dollarSymbol = "$";

    const std::string rubbleSymbol = "₽";

    const std::string euroSymbol = "€";

    /**
     *
     */
    enum class CurrencyType {
        /**
         *
         */
        Dollar,

        /**
         *
         */
        Euro,

        /**
         *
         */
        Ruble
    };

    /**
     *
     */
    class MoneyPresentation {
    public:
        /**
         *
         */
        enum class Rules {
            /**
             *
             */
            Penny = 0b001,

            /**
             *
             */
            CurrencySymbol = 0b010
        };

        /**
         *
         * @param value
         * @param format
         * @param currencyType
         * @return
         */
        [[nodiscard]] static std::string FormatMoney(std::string value,
                                                     Rules format,
                                                     CurrencyType currencyType) noexcept;

        /**
         *
         * @param str
         * @return
         */
        [[nodiscard]] static bool IsNumber(const std::string &str) noexcept;
    };
}
