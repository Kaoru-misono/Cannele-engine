#pragma once

namespace bi {
    template <typename T>
    using Ref = T*;

    struct MoveOnly {
    MoveOnly() noexcept = default;

    MoveOnly(MoveOnly&& rhs) noexcept = default;
    MoveOnly(MoveOnly const& rhs) noexcept = delete;
    auto operator=(MoveOnly&& rhs) noexcept -> MoveOnly& = default;
    auto operator=(MoveOnly const& rhs) noexcept -> MoveOnly& = delete;

    auto operator==(MoveOnly const& rhs) const noexcept -> bool = default;
    auto operator<=>(MoveOnly const& rhs) const noexcept = default;
    };

template <typename T>
struct Box final : MoveOnly {
    Box() noexcept = default;

    template <typename... Args> requires std::constructible_from<T, Args...>
    static auto make(Args&&... args) noexcept -> Box {
        return Box(new T(std::forward<Args>(args)...));
    }

    static auto unsafe_make(T* ptr) noexcept -> Box {
        assert(ptr && "initialize Box from null pointer");
        return Box(ptr);
    }

    ~Box() noexcept { reset(nullptr); }

    Box(Box&& rhs) noexcept : ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

    template <typename T2> requires std::convertible_to<T2*, T*>
    Box(Box<T2> &&rhs) noexcept : ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

    auto operator=(Box &&rhs) noexcept -> Box& {
        if (this != std::addressof(rhs)) {
            reset(rhs.release());
        }
        return *this;
    }

    template <typename T2> requires std::convertible_to<T2*, T*>
    auto operator=(Box<T2>&& rhs) noexcept -> Box& {
        reset(rhs.release());
        return *this;
    }

    auto operator==(Box const& rhs) const noexcept -> bool = default;
    auto operator<=>(Box const& rhs) const noexcept = default;

    operator bool() const noexcept { return ptr_ != nullptr; }

    auto operator*() const noexcept(noexcept(*std::declval<T*>())) -> std::add_lvalue_reference_t<T> {
        return *ptr_;
    }
    auto operator->() const noexcept -> T* { return ptr_; }
    auto get() const noexcept -> T* { return ptr_; }

    auto ref() const noexcept -> Ref<T> {
        return Ref<T>::unsafe_make(ptr_);
    }

    void reset(T* ptr = nullptr) {
        if (ptr_) {
            delete ptr_;
        }
        ptr_ = ptr;
    }

private:
    explicit Box(T* ptr) noexcept : ptr_(ptr) {}

    auto release() -> T* {
        auto ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }

    T* ptr_ = nullptr;
};

template <typename T>
struct Box<T[]> final : MoveOnly {
    Box() noexcept = default;

    static auto make(size_t size) noexcept -> Box {
        return Box(new T[size]);
    }

    static auto unsafe_make(T* ptr) noexcept -> Box {
        assert(ptr && "initialize Box from null pointer");
        return Box(ptr);
    }

    ~Box() noexcept { reset(nullptr); }

    Box(Box&& rhs) noexcept : ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

    template <typename T2> requires std::convertible_to<T2*, T*>
    Box(Box<T2[]>&& rhs) noexcept : ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

    auto operator=(Box &&rhs) noexcept -> Box& {
        if (this != std::addressof(rhs)) {
            reset(rhs.release());
        }
        return *this;
    }

    template <typename T2> requires std::convertible_to<T2*, T*>
    auto operator=(Box<T2[]>&& rhs) noexcept -> Box& {
        reset(rhs.release());
        return *this;
    }

    auto operator==(Box const& rhs) const noexcept -> bool = default;
    auto operator<=>(Box const& rhs) const noexcept = default;

    operator bool() const noexcept { return ptr_ != nullptr; }

    auto operator*() const noexcept(noexcept(*std::declval<T*>())) -> std::add_lvalue_reference_t<T> {
        return *ptr_;
    }
    auto operator->() const noexcept -> T* { return ptr_; }
    auto get() const noexcept -> T* { return ptr_; }

    auto operator[](size_t index) noexcept -> T& { return ptr_[index]; }
    auto operator[](size_t index) const noexcept -> const T& { return ptr_[index]; }

    auto ref() const noexcept -> Ref<T> {
        return Ref<T>::unsafe_make(ptr_);
    }

    void reset(T* ptr = nullptr) {
        if (ptr_) {
            delete[] ptr_;
        }
        ptr_ = ptr;
    }

private:
    explicit Box(T* ptr) noexcept : ptr_(ptr) {}

    auto release() -> T* {
        auto ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }


    T* ptr_ = nullptr;
};

}

