#include "dynamic_array.hpp"

template <class T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    if (count < 0)
        throw std::invalid_argument("Negative size not allowed");

    size = count;
    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = items[i];
}

template <class T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0)
        throw std::invalid_argument("Negative size not allowed");

    this->size = size;
    data = new T[size];
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
    size = other.size;
    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
}

template <class T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <class T>
T DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of bounds");
    return data[index];
}

template <class T>
int DynamicArray<T>::GetSize() const {
    return size;
}

template <class T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of bounds");
    data[index] = value;
}

template <class T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0)
        throw std::invalid_argument("Negative size not allowed");

    T* newData = new T[newSize];
    int minSize = (newSize < size) ? newSize : size;

    for (int i = 0; i < minSize; i++)
        newData[i] = data[i];

    delete[] data;
    data = newData;
    size = newSize;
}

template <class T>
T& DynamicArray<T>::operator[](int index) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of bounds");
    return data[index];
}

template <class T>
const T& DynamicArray<T>::operator[](int index) const {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of bounds");
    return data[index];
}
