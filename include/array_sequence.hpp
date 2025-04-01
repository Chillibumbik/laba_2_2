#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"
#include <stdexcept>

// Изменяемая версия — базовый класс

template <typename T>
class MutableArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;

    Sequence<T>* CreateFromArray(DynamicArray<T>* array) const {
        return new MutableArraySequence<T>(*array);
    }

public:
    MutableArraySequence() {
        items = new DynamicArray<T>(0);
    }

    MutableArraySequence(T* arr, int count) {
        items = new DynamicArray<T>(arr, count);
    }

    MutableArraySequence(const MutableArraySequence<T>& other) {
        items = new DynamicArray<T>(*other.items);
    }

    MutableArraySequence(const DynamicArray<T>& array) {
        items = new DynamicArray<T>(array);
    }

    ~MutableArraySequence() override {
        delete items;
    }

    T GetFirst() const override {
        if (GetLength() == 0) throw std::out_of_range("Empty array");
        return items->Get(0);
    }

    T GetLast() const override {
        if (GetLength() == 0) throw std::out_of_range("Empty array");
        return items->Get(GetLength() - 1);
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        DynamicArray<T>* sub = items->GetSubArray(startIndex, endIndex);
        return CreateFromArray(sub);
    }

    Sequence<T>* Concat(Sequence<T>* other) const override {
        auto otherArray = dynamic_cast<const MutableArraySequence<T>*>(other);
        if (!otherArray) throw std::invalid_argument("Incompatible sequence types");

        int totalSize = GetLength() + otherArray->GetLength();
        DynamicArray<T>* result = new DynamicArray<T>(totalSize);

        for (int i = 0; i < GetLength(); i++) result->Set(i, items->Get(i));
        for (int j = 0; j < otherArray->GetLength(); j++) result->Set(j + GetLength(), otherArray->Get(j));

        return CreateFromArray(result);
    }

    Sequence<T>* Append(T item) override {
        int size = items->GetSize();
        items->Resize(size + 1);
        items->Set(size, item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        int size = items->GetSize();
        items->Resize(size + 1);
        for (int i = size; i > 0; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        int size = items->GetSize();
        if (index < 0 || index > size) throw std::out_of_range("Invalid index");
        items->Resize(size + 1);
        for (int i = size; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        return this;
    }

    Sequence<T>* AppendInternal(T item) override { return Append(item); }
    Sequence<T>* PrependInternal(T item) override { return Prepend(item); }
    Sequence<T>* InsertAtInternal(T item, int index) override { return InsertAt(item, index); }

    Sequence<T>* Instance() override { return this; }
    Sequence<T>* Clone() const override { return new MutableArraySequence<T>(*this); }
};

// Неизменяемая версия

template <typename T>
class ImmutableArraySequence : public MutableArraySequence<T> {
public:
    using MutableArraySequence<T>::MutableArraySequence;

    Sequence<T>* Append(T item) override {
        return this->Clone()->AppendInternal(item);
    }

    Sequence<T>* Prepend(T item) override {
        return this->Clone()->PrependInternal(item);
    }

    Sequence<T>* InsertAt(T item, int index) override {
        return this->Clone()->InsertAtInternal(item, index);
    }

    Sequence<T>* AppendInternal(T) override { throw std::logic_error("Immutable"); }
    Sequence<T>* PrependInternal(T) override { throw std::logic_error("Immutable"); }
    Sequence<T>* InsertAtInternal(T, int) override { throw std::logic_error("Immutable"); }

    Sequence<T>* Instance() override { return this->Clone(); }
    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};
