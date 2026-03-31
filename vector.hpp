#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <memory>

namespace sjtu {

template<typename T>
class vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t new_capacity) {
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

        if (data_) {
            // Move or copy construct elements
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(new_data, data_, size_ * sizeof(T));
            } else {
                for (size_t i = 0; i < size_; ++i) {
                    new (new_data + i) T(std::move(data_[i]));
                    data_[i].~T();
                }
            }
            ::operator delete(data_);
        }

        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    // Constructors
    vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit vector(size_t n) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(::operator new(n * sizeof(T)));
            capacity_ = n;
            size_ = n;
            for (size_t i = 0; i < n; ++i) {
                new (data_ + i) T();
            }
        }
    }

    vector(size_t n, const T& value) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(::operator new(n * sizeof(T)));
            capacity_ = n;
            size_ = n;
            for (size_t i = 0; i < n; ++i) {
                new (data_ + i) T(value);
            }
        }
    }

    vector(const vector& other) : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ > 0) {
            data_ = static_cast<T*>(::operator new(other.size_ * sizeof(T)));
            capacity_ = other.size_;
            size_ = other.size_;

            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(data_, other.data_, size_ * sizeof(T));
            } else {
                for (size_t i = 0; i < size_; ++i) {
                    new (data_ + i) T(other.data_[i]);
                }
            }
        }
    }

    vector(vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~vector() {
        if (data_) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~T();
                }
            }
            ::operator delete(data_);
        }
    }

    // Assignment operators
    vector& operator=(const vector& other) {
        if (this != &other) {
            if (data_) {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    for (size_t i = 0; i < size_; ++i) {
                        data_[i].~T();
                    }
                }
                ::operator delete(data_);
            }

            if (other.size_ > 0) {
                data_ = static_cast<T*>(::operator new(other.size_ * sizeof(T)));
                capacity_ = other.size_;
                size_ = other.size_;

                if constexpr (std::is_trivially_copyable_v<T>) {
                    std::memcpy(data_, other.data_, size_ * sizeof(T));
                } else {
                    for (size_t i = 0; i < size_; ++i) {
                        new (data_ + i) T(other.data_[i]);
                    }
                }
            } else {
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
            }
        }
        return *this;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            if (data_) {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    for (size_t i = 0; i < size_; ++i) {
                        data_[i].~T();
                    }
                }
                ::operator delete(data_);
            }

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Element access
    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("vector::at");
        }
        return data_[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("vector::at");
        }
        return data_[index];
    }

    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }
    T& back() { return data_[size_ - 1]; }
    const T& back() const { return data_[size_ - 1]; }

    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }

    // Capacity
    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }

    void reserve(size_t new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            if (size_ == 0) {
                ::operator delete(data_);
                data_ = nullptr;
                capacity_ = 0;
            } else {
                reallocate(size_);
            }
        }
    }

    // Modifiers
    void clear() noexcept {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = 0;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        new (data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            if constexpr (!std::is_trivially_destructible_v<T>) {
                data_[size_].~T();
            }
        }
    }

    void resize(size_t new_size) {
        if (new_size > capacity_) {
            reallocate(new_size);
        }

        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                new (data_ + i) T();
            }
        } else if (new_size < size_) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = new_size; i < size_; ++i) {
                    data_[i].~T();
                }
            }
        }
        size_ = new_size;
    }

    void resize(size_t new_size, const T& value) {
        if (new_size > capacity_) {
            reallocate(new_size);
        }

        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                new (data_ + i) T(value);
            }
        } else if (new_size < size_) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = new_size; i < size_; ++i) {
                    data_[i].~T();
                }
            }
        }
        size_ = new_size;
    }

    // Iterators
    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }
};

} // namespace sjtu

#endif // SJTU_VECTOR_HPP
