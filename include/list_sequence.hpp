#pragma once
#include "sequence.hpp"
#include "linked_list.hpp"

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;

public:
    ListSequence();
    ListSequence(T* items, int count);
    ListSequence(const ListSequence<T>& other);
    ListSequence(const LinkedList<T>& otherList);
    ~ListSequence();

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
class MutableListSequence : public ListSequence<T> {
public:
    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }

    Sequence<T>* Instance() override {
        return this;
    }
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* Instance() override {
        return this->Clone();
    }
};
