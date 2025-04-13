#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "errors.hpp"
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
        if (GetLength() == 0) throw Errors::EmptyArray();
        return items->Get(0);
    }

    T GetLast() const override {
        if (GetLength() == 0) throw Errors::EmptyArray();
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
        auto* result = new MutableArraySequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        auto otherArray = dynamic_cast<const MutableArraySequence<T>*>(other);
        if (!otherArray) throw Errors::IncompatibleTypes();

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
        if (index < 0 || index > size) throw Errors::IndexOutOfRange();
        items->Resize(size + 1);
        for (int i = size; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        return this;
    }

    Sequence<T>* Remove(int index) override {
        if (items->GetSize() != 0 ){
            items->Remove(index);
            return this;
        } else{
            throw Errors::EmptyArray();
        }
    }

    Sequence<T>* Instance() override { return this; }
    Sequence<T>* Clone() const override { return new MutableArraySequence<T>(*this); }
};

template <typename T>
MutableArraySequence<T> operator+(const MutableArraySequence<T>& lhs, const MutableArraySequence<T>& rhs) {
    Sequence<T>* resultBase = lhs.Concat(&rhs);
    auto* result = static_cast<MutableArraySequence<T>*>(resultBase);
    MutableArraySequence<T> copy(*result); 
    delete result;
    return copy;
}


// Неизменяемая версия

template <typename T>
class ImmutableArraySequence : public MutableArraySequence<T> {
public:
    using MutableArraySequence<T>::MutableArraySequence;

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        const auto* otherArr = dynamic_cast<const ImmutableArraySequence<T>*>(other);
        if (!otherArr) throw Errors::IncompatibleTypes();
    
        int totalSize = this->GetLength() + otherArr->GetLength();
        DynamicArray<T> combined(totalSize);
    
        for (int i = 0; i < this->GetLength(); ++i)
            combined.Set(i, this->Get(i));
        for (int j = 0; j < otherArr->GetLength(); ++j)
            combined.Set(j + this->GetLength(), otherArr->Get(j));
    
        return new ImmutableArraySequence<T>(combined);
    }

    Sequence<T>* Append(T item) override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        clone->MutableArraySequence<T>::Append(item); 
        return clone;   
    }

    Sequence<T>* Prepend(T item) override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        clone->MutableArraySequence<T>::Prepend(item); 
        return clone;

    }

    Sequence<T>* InsertAt(T item, int index) override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        clone->MutableArraySequence<T>::InsertAt(item, index); 
        return clone;
        }


    Sequence<T>* Remove(int index) override {
        return this->Clone()->Remove(index);
    }

    Sequence<T>* Instance() override { return this->Clone(); }
    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

template <typename T>
ImmutableArraySequence<T> operator+(const ImmutableArraySequence<T>& lhs, const ImmutableArraySequence<T>& rhs) {
    Sequence<T>* resultBase = lhs.Concat(&rhs);  
    auto* result = dynamic_cast<ImmutableArraySequence<T>*>(resultBase);
    if (!result) throw std::runtime_error("Invalid Concat result type");
    ImmutableArraySequence<T> copy(*result);
    delete result;
    return copy;
}
