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
        Ruble,
    };

    /**
     *
     */
    enum class FormatMoneyRules {
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
     * @param str
     * @return
     */
    [[nodiscard]] bool IsNumber(const std::string &str) {
        const auto nonDigitSymbol = std::find_if(std::begin(str), std::end(str),
                                                 [](unsigned char c) {
                                                     return !std::isdigit(c);
                                                 });
        const bool found = nonDigitSymbol != std::end(str);
        return !str.empty() && !found;
    }

    /**
     *
     * @param value
     * @param format
     * @param currencyType
     * @return
     */
    [[nodiscard]] std::string FormatMoney(std::string value,
                                          int format,
                                          CurrencyType currencyType) noexcept {
        const std::string originValue = value;
        std::string currencySymbol;
        switch (currencyType) {
            case CurrencyType::Dollar:
                currencySymbol = dollarSymbol;
                break;
            case CurrencyType::Ruble:
                currencySymbol = rubbleSymbol;
                break;
            case CurrencyType::Euro:
                currencySymbol = euroSymbol;
                break;
            default:
                break;
        }
        if (value.empty()) {
            return {};
        }
        std::replace(std::begin(value), std::begin(value), ' ', '\0');
        std::string pennyPart = "00";

        const std::int16_t currencyIndex = value.find(currencySymbol);
        if (currencyIndex != std::string::npos) {
            value = value.substr(0, currencyIndex + 1);
        }

        std::int16_t pennyIndex = value.find(',');
        if (pennyIndex == std::string::npos) {
            pennyIndex = value.find('.');
        }
        if (pennyIndex != std::string::npos) {
            pennyPart = value.substr(pennyIndex + 1, std::size(value));
            value = value.substr(0, pennyIndex);
        }

        if (!IsNumber(value)) {
            return originValue;
        }

        auto endIterator = std::end(value);
        while ((endIterator - std::cbegin(value)) > 3) {
            value.insert(endIterator - 3, ' ');
            endIterator -= 3;
        }

        const bool usePenny = static_cast<int>(format) & 0b001;
        const bool useCurrencySymbol = static_cast<int>(format) & 0b010;
        std::string resultView = value;
        if (usePenny) {
            resultView += "." + pennyPart;
        }
        if (useCurrencySymbol) {
            resultView += " " + currencySymbol;
        }
        return resultView;
    }
}
