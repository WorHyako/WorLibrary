#ifndef WORLIBRARY_VECTOR_VECTOR2_HPP
#define WORLIBRARY_VECTOR_VECTOR2_HPP

namespace wor::Vector {

    template<typename T>
    struct Vector2 {

        Vector2() noexcept;

        Vector2(T x_, T y_) noexcept;

        T x, y;

#pragma region Operators

        friend bool operator<(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept;

        friend bool operator>(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept;

        friend bool operator<=(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept;

        friend bool operator>=(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept;

        Vector2<T> &operator*=(T rhs_) noexcept;

        Vector2<T> &operator*=(const Vector2<T> &rhs_) noexcept;

        Vector2<T> &operator/=(T rhs_) noexcept;

        Vector2<T> &operator/=(const Vector2<T> &rhs_) noexcept;

        Vector2<T> &operator+=(T rhs_) noexcept;

        Vector2<T> &operator+=(const Vector2<T> &rhs_) noexcept;

        Vector2<T> &operator-=(T rhs_) noexcept;

        Vector2<T> &operator-=(const Vector2<T> &rhs_) noexcept;

        Vector2<T> &operator=(T rhs_) noexcept;

        friend Vector2<T> operator+(Vector2<T> lhs_, Vector2<T> rhs_) noexcept;

        friend Vector2<T> operator+(Vector2<T> lhs_, T rhs_) noexcept;

        friend Vector2<T> operator-(Vector2<T> lhs_, T rhs_) noexcept;

        friend Vector2<T> operator-(Vector2<T> lhs_, Vector2<T> rhs_) noexcept;

        bool operator==(const Vector2<T> &rhs_) const noexcept;

        bool operator!=(const Vector2<T> &rhs_) const noexcept;

#pragma endregion Operators

    };

    template<typename T>
    Vector2<T>::Vector2() noexcept {
        x = static_cast<T>(0);
        y = static_cast<T>(0);
    }

    template<typename T>
    Vector2<T>::Vector2(T x_, T y_) noexcept
            : x(x_), y(y_) {}

#pragma region Operators

    template<typename T>
    bool operator<(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept {
        return lhs_.x < rhs_.x
               && lhs_.y < rhs_.y;
    }

    template<typename T>
    bool operator>(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept {
        return rhs_ < lhs_;
    }

    template<typename T>
    bool operator>=(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept {
        return !(lhs_ < rhs_);
    }

    template<typename T>
    bool operator<=(const Vector2<T> &lhs_, const Vector2<T> &rhs_) noexcept {
        return !(lhs_ > rhs_);
    }

    template<typename T>
    bool Vector2<T>::operator==(const Vector2<T> &rhs_) const noexcept {
        return x == rhs_.x
               && y == rhs_.y;
    }

    template<typename T>
    bool Vector2<T>::operator!=(const Vector2<T> &rhs_) const noexcept {
        return x != rhs_.x
               && y != rhs_.y;
    }

    template<typename T>
    Vector2<T> operator+(Vector2<T> lhs_, T rhs_) noexcept {
        lhs_ += rhs_;
        return lhs_;
    }

    template<typename T>
    Vector2<T> operator+(Vector2<T> lhs_, Vector2<T> rhs_) noexcept {
        lhs_ += rhs_;
        return lhs_;
    }

    template<typename T>
    Vector2<T> operator-(Vector2<T> lhs_, T rhs_) noexcept {
        return lhs_ -= rhs_;
    }

    template<typename T>
    Vector2<T> operator-(Vector2<T> lhs_, Vector2<T> rhs_) noexcept {
        return lhs_ -= rhs_;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator*=(const Vector2<T> &rhs_) noexcept {
        x *= rhs_.x;
        y *= rhs_.y;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator*=(T rhs_) noexcept {
        x *= rhs_;
        y *= rhs_;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator/=(const Vector2<T> &rhs_) noexcept {
        x /= rhs_.x;
        y /= rhs_.y;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator/=(T rhs_) noexcept {
        x /= rhs_;
        y /= rhs_;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator+=(const Vector2<T> &rhs_) noexcept {
        x += rhs_.x;
        y += rhs_.y;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator+=(T rhs_) noexcept {
        x += rhs_;
        y += rhs_;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator-=(T rhs_) noexcept {
        x -= rhs_;
        y -= rhs_;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator-=(const Vector2<T> &rhs_) noexcept {
        x -= rhs_.x;
        y -= rhs_.y;
        return *this;
    }

    template<typename T>
    Vector2<T> &Vector2<T>::operator=(T rhs_) noexcept {
        x = rhs_;
        y = rhs_;
        return *this;
    }

#pragma endregion Operators
}

#endif
