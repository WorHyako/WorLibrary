#include "WorLibrary/Currency/MoneyStrPresentation.hpp"

using namespace Wor::Currency;

std::string MoneyPresentation::formatMoney(std::string value,
                                           Rules format,
                                           CurrencyType currencyType) noexcept {
    std::string originValue = value;
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

    auto pennyIndex = static_cast<std::int16_t>(value.find(','));
    if (pennyIndex == std::string::npos) {
        pennyIndex = static_cast<std::int16_t>(value.find('.'));
    }
    if (pennyIndex != std::string::npos) {
        pennyPart = value.substr(pennyIndex + 1, std::size(value));
        value = value.substr(0, pennyIndex);
    }

    if (!isNumber(value)) {
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

bool MoneyPresentation::isNumber(const std::string &str) noexcept {
    const auto nonDigitSymbol = std::find_if(std::begin(str), std::end(str),
                                             [](unsigned char c) {
                                                 return !std::isdigit(c);
                                             });
    const bool found = nonDigitSymbol != std::end(str);
    return !str.empty() && !found;
}
