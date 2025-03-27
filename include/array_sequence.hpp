#pragma once
#include "sequence.hpp"
#include "dynamic_array.hpp"

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;

public:
    ArraySequence();
    ArraySequence(T* items, int count);
    ArraySequence(const ArraySequence<T>& other);
    ~ArraySequence();

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;

    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, int index) override;

    Sequence<T>* Concat(Sequence<T>* list) override;

    virtual Sequence<T>* Clone() const = 0;
    virtual Sequence<T>* Instance() = 0;
};


template <class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }

    Sequence<T>* Instance() override {
        return this;
    }
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* Instance() override {
        return this->Clone();
    }
};
