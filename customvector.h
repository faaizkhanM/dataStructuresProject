#ifndef CUSTOMVECTOR_H
#define CUSTOMVECTOR_H

#include <stdexcept>

template<typename T>
class CustomVector {
private:
    T* data;
    size_t size_;
    size_t capacity_;

    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity_ = newCapacity;
    }

public:
    // Constructor
    CustomVector() : data(nullptr), size_(0), capacity_(0) {}
    
    // Constructor with size
    explicit CustomVector(size_t size) : data(new T[size]), size_(size), capacity_(size) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = T(); // Default initialize elements
        }
    }

    // Destructor
    ~CustomVector() {
        delete[] data;
    }

    // Copy constructor
    CustomVector(const CustomVector& other) : size_(other.size_), capacity_(other.capacity_) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Element access
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Capacity
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Modifiers
    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[size_++] = value;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    void clear() {
        size_ = 0;
    }

    // Iterator support
    T* begin() { return data; }
    T* end() { return data + size_; }
    const T* begin() const { return data; }
    const T* end() const { return data + size_; }
};

#endif // CUSTOMVECTOR_H 