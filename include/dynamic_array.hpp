#pragma once
#include <stdexcept>

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray();

    T Get(int index) const;
    int GetSize() const;

    void Set(int index, T value);
    void Resize(int newSize);
};
