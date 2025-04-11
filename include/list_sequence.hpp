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
        if (list->GetLength() != 0 ){
            list->Remove(index);
            return this;
        } else{
            throw Errors::EmptyList();
        }
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
     Sequence<T>* resultBase = lhs.Concat(&rhs); // вернёт через CreateFromList
     auto* result = static_cast<MutableListSequence<T>*>(resultBase); // cast
     MutableListSequence<T> copy = *result;  // копируем в объект
     delete result;                          // освобождаем временный
     return copy;
 }

// Неизменяемая версия

template <typename T>
class ImmutableListSequence : public MutableListSequence<T> {
public:
    using MutableListSequence<T>::MutableListSequence;

    Sequence<T>* Append(T item) override {
        return this->Clone()->Append(item);
    }

    Sequence<T>* Prepend(T item) override {
        return this->Clone()->Prepend(item);
    }

    Sequence<T>* InsertAt(T item, int index) override {
        return this->Clone()->InsertAt(item, index);
    }

    Sequence<T>* Remove(int index) override {
        return this->Clone()->Remove(index);
    }


    Sequence<T>* Instance() override { return this->Clone(); }
    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

template <typename T>
ImmutableListSequence<T> operator+(const ImmutableListSequence<T>& lhs, const ImmutableListSequence<T>& rhs) {
    Sequence<T>* resultBase = lhs.Concat(&rhs); // вернёт через CreateFromList
    auto* result = static_cast<ImmutableListSequence<T>*>(resultBase); // cast
    ImmutableListSequence<T> copy = *result;  // копируем в объект
    delete result;                          // освобождаем временный
    return copy;
}