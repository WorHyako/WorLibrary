#ifndef WORLIBRARY_VECTOR_VECTOR_HPP
#define WORLIBRARY_VECTOR_VECTOR_HPP

namespace wor::Vector {

    /**
     * Struct to store int or float data
     * @tparam T int/float value
     */
    template<typename T>
    struct Vector {
        Vector() noexcept = default;

        virtual ~Vector() = default;
    };
}
#endif
