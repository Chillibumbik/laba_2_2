#pragma once

#include "sequence.hpp"
#include "linked_list.hpp"
#include <stdexcept>

// Изменяемая версия — базовый класс

template <typename T>
class MutableListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;

    Sequence<T>* CreateFromList(LinkedList<T>* list) const {
        return new MutableListSequence<T>(*list);
    }

public:
    MutableListSequence() {
        list = new LinkedList<T>();
    }

    MutableListSequence(T* items, int count) {
        list = new LinkedList<T>(items, count);
    }

    MutableListSequence(const MutableListSequence<T>& other) {
        list = new LinkedList<T>(*other.list);
    }

    MutableListSequence(const LinkedList<T>& list) {
        this->list = new LinkedList<T>(list);
    }

    ~MutableListSequence() override {
        delete list;
    }

    T GetFirst() const override { return list->GetFirst(); }
    T GetLast() const override { return list->GetLast(); }
    T Get(int index) const override { return list->Get(index); }
    int GetLength() const override { return list->GetLength(); }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* sub = list->GetSubList(startIndex, endIndex);
        return CreateFromList(sub);
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        auto otherList = dynamic_cast<const MutableListSequence<T>*>(other);
        if (!otherList) throw Errors::IncompatibleTypes();
        LinkedList<T>* result = list->Concat(otherList->list);
        return CreateFromList(result);
    }

    Sequence<T>* Append(T item) override {
        list->Append(item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        list->Prepend(item);
        return this;
    }

    Sequence<T>* Remove(int index) override {
        list->Remove(index);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        list->InsertAt(item, index);
        return this;
    }

    Sequence<T>* Instance() override { return this; }
    Sequence<T>* Clone() const override { return new MutableListSequence<T>(*this); }

};

/* template <typename T>
MutableListSequence<T> operator+(const MutableListSequence<T>& lhs, const MutableListSequence<T>& rhs) {
    MutableListSequence<T> result(lhs);  
    for (int i = 0; i < rhs.GetLength(); ++i) {
        result.Append(rhs.Get(i));   
    }
    return result;
}
 */

 template <typename T>
MutableListSequence<T> operator+(const MutableListSequence<T>& lhs, const MutableListSequence<T>& rhs) {
    Sequence<T>* result = lhs.Concat(&rhs); 
    return result;  
}

// Неизменяемая версия

template <typename T>
class ImmutableListSequence : public MutableListSequence<T> {
public:
    using MutableListSequence<T>::MutableListSequence;

    Sequence<T>* Append(T item) override {
        return this->Clone()->AppendInternal(item);
    }

    Sequence<T>* Prepend(T item) override {
        return this->Clone()->PrependInternal(item);
    }

    Sequence<T>* InsertAt(T item, int index) override {
        return this->Clone()->InsertAtInternal(item, index);
    }

    Sequence<T>* Remove(int index) override {
        return this->Clone()->Remove(index);
    }


    Sequence<T>* Instance() override { return this->Clone(); }
    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};
