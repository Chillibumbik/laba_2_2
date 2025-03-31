#pragma once

#include "sequence.hpp"
#include "linked_list.hpp"

// Базовый класс ListSequence

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;

    ListSequence(LinkedList<T>* list) : list(list) {}

public:
    ListSequence() {
        list = new LinkedList<T>();
    }

    ListSequence(T* items, int count) {
        list = new LinkedList<T>(items, count);
    }

    ListSequence(const ListSequence<T>& other) {
        list = new LinkedList<T>(*other.list);
    }

    ListSequence(const LinkedList<T>& list) {
        this->list = new LinkedList<T>(list);
    }

    ~ListSequence() override {
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

    Sequence<T>* Concat(Sequence<T>* other) const override {
        auto otherList = dynamic_cast<const ListSequence<T>*>(other);
        if (!otherList) throw std::invalid_argument("Incompatible sequence types");
        LinkedList<T>* result = list->Concat(otherList->list);
        return CreateFromList(result);
    }

    virtual Sequence<T>* CreateFromList(LinkedList<T>* list) const = 0;
};

// Immutable

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(T* items, int count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}
    ImmutableListSequence(const LinkedList<T>& list) : ListSequence<T>(list) {}


    Sequence<T>* Append(T item) override {
        auto copy = new LinkedList<T>(*this->list);
        copy->Append(item);
        return new ImmutableListSequence<T>(*copy);
    }

    Sequence<T>* Prepend(T item) override {
        auto copy = new LinkedList<T>(*this->list);
        copy->Prepend(item);
        return new ImmutableListSequence<T>(*copy);
    }

    Sequence<T>* InsertAt(T item, int index) override {
        auto copy = new LinkedList<T>(*this->list);
        copy->InsertAt(item, index);
        return new ImmutableListSequence<T>(*copy);
    }

    Sequence<T>* AppendInternal(T) override { throw std::logic_error("Immutable"); }
    Sequence<T>* PrependInternal(T) override { throw std::logic_error("Immutable"); }
    Sequence<T>* InsertAtInternal(T, int) override { throw std::logic_error("Immutable"); }

    Sequence<T>* Instance() override { return Clone(); }
    Sequence<T>* Clone() const override { return new ImmutableListSequence<T>(*this); }

    Sequence<T>* CreateFromList(LinkedList<T>* list) const override {
        return new ImmutableListSequence<T>(*list);
    }
};

// Mutable

template <class T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(T* items, int count) : ListSequence<T>(items, count) {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {}
    MutableListSequence(const LinkedList<T>& list) : ListSequence<T>(list) {}


    Sequence<T>* Append(T item) override {
        this->list->Append(item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        this->list->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        this->list->InsertAt(item, index);
        return this;
    }

    Sequence<T>* AppendInternal(T item) override { return Append(item); }
    Sequence<T>* PrependInternal(T item) override { return Prepend(item); }
    Sequence<T>* InsertAtInternal(T item, int index) override { return InsertAt(item, index); }

    Sequence<T>* Instance() override { return this; }
    Sequence<T>* Clone() const override { return new MutableListSequence<T>(*this); }

    Sequence<T>* CreateFromList(LinkedList<T>* list) const override {
        return new MutableListSequence<T>(*list);
    }
};
