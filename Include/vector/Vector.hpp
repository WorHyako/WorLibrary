#ifndef WORLIBRARY_VECTOR_VECTOR_HPP
#define WORLIBRARY_VECTOR_VECTOR_HPP

#include <type_traits>

namespace wor::Vector {

    template<typename T>
    const bool isIntOrFloat = std::is_same<T, int>::value || std::is_same<T, float>::value;

    /**
     * Struct to store int or float data
     * @tparam T int/float value
     */
    template<typename T>
    struct Vector {
        static_assert(isIntOrFloat<T>, L"Wrong type. Use only int/float");

        Vector() noexcept = default;

        virtual ~Vector() = default;
    };
}
#endif
