#pragma once

#include "container.hpp"

namespace cannele::core
{
    template <class T>
    struct Span final
    {
        using value_type = T;
        using const_value_type = value_type const;

        using pointer = value_type*;
        using const_pointer = const_value_type*;

        using reference = value_type&;
        using const_reference = const_value_type&;

        constexpr Span() = default;
        ~Span() = default;

        template <class Iter>
        constexpr Span(Iter first, Iter last) noexcept
            // Use "&*iter" instead of "iter" to make sure that iterators are converted to pointers.
            : first{&*first}, last{&*last}
        {}

        Span(T const* data, size_t size) : first(data), last(data + size) {}

        Span(Init_List<T> list) : Span{list.begin(), list.end()} {}

        template <size_t N>
        Span(Array<T, N> const& array) : Span{array.begin(), array.end()} {}

        Span(Vector<T> const& vector) : Span{vector.begin(), vector.end()} {}

        auto data() const -> const_pointer { return first; }

        auto empty() const -> bool { return (first >= last); }

        auto size() const -> size_t { return (empty() ? size_t{} : size_t(last - first)); }

        auto operator[](size_t index) const -> const_reference { return first[index]; }

        auto begin() const -> pointer { return first; }
        auto end() const -> pointer { return last; }
        auto rbegin() const -> const_pointer { return last; }
        auto rend() const -> const_pointer { return last; }

    private:
        pointer first{};
        pointer last{};
    };

    template <class T, class U>
    auto operator == (Span<T> a, Span<U> b) -> bool
    {
        if (a.size() != b.size())
            return false;

        auto p = b.begin();
        for (auto& x: a)
            if (!(x == *p++))
                return false;

        return true;
    }

    template <class T, class U>
    auto operator != (Span<T> a, Span<U> b) -> bool
    {
        return !(a == b);
    }
}
