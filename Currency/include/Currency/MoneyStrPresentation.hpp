#pragma once

#include <string>
#include <cstdint>

namespace Wor::Currency {

    constexpr std::string_view dollarSymbol = "$";

    constexpr std::string_view rubbleSymbol = "₽";

    constexpr std::string_view euroSymbol = "€";

    /**
     * @brief
     *
     * @author WorHyako
     */
    enum class CurrencyType
            : std::uint8_t {
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
     * @brief
     *
     * @author WorHyako
     */
    class MoneyPresentation final {
    public:
        /**
         *
         */
        enum class Rules
                : std::uint8_t {
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
         * @brief
         *
         * @param value
         *
         * @param format
         *
         * @param currencyType
         *
         * @return
         */
        [[nodiscard]]
        static std::string formatMoney(std::string value,
                                       Rules format,
                                       CurrencyType currencyType) noexcept;

        /**
         * @brief
         *
         * @param str
         *
         * @return
         */
        [[nodiscard]]
        static bool isNumber(const std::string &str) noexcept;
    };
}
