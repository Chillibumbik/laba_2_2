#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "boxed_any.hpp"

/*
Макрос	Что делает
REQUIRE(x)	Проверяет, что x == true. Если нет — стоп.
CHECK(x)	То же, но не останавливает тесты
REQUIRE_THROWS(expr)	Проверяет, что expr выбрасывает исключение
REQUIRE_THROWS_AS(expr, Type)	То же + проверка типа исключения
REQUIRE_FALSE(x)	Проверка, что x == false
*/

TEST_CASE("DynamicArray: Basic Operations", "[DynamicArray]") {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);

    //REQUIRE(arr.GetCapacity() == 6);
    REQUIRE(arr.GetSize() == 3);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr.Get(1) == 20);
    REQUIRE(arr.Get(2) == 30);

    arr.Resize(5);
    REQUIRE(arr.GetSize() == 5);
    //REQUIRE(arr.GetCapacity() == 10);

    arr.Resize(2);
    REQUIRE(arr.GetSize() == 2);
    REQUIRE(arr.Get(0) == 10);
    REQUIRE(arr.Get(1) == 20);

    REQUIRE_THROWS_AS(arr.Get(10), std::out_of_range);
    REQUIRE_THROWS_AS(arr.Set(5, 100), std::out_of_range);

    arr.Remove(1);
    REQUIRE(arr.GetSize() == 1);
}

TEST_CASE("LinkedList: Basic Operations", "[LinkedList]") {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Prepend(0);

    REQUIRE(list.GetLength() == 3);
    REQUIRE(list.GetFirst() == 0);
    REQUIRE(list.GetLast() == 2);
    REQUIRE(list.Get(1) == 1);

    REQUIRE(list.GetTail() == 2);

    list.InsertAt(5, 1);
    REQUIRE(list.Get(1) == 5);
    REQUIRE(list.Get(2) == 1);

    LinkedList<int>* sub = list.GetSubList(1, 2);
    REQUIRE(sub->GetLength() == 2);
    REQUIRE(sub->Get(0) == 5);
    REQUIRE(sub->Get(1) == 1);

    REQUIRE_THROWS_AS(list.Get(10), std::out_of_range);
    REQUIRE_THROWS_AS(list.GetSubList(0, 10), std::out_of_range);
}

TEST_CASE("ArraySequence: Operations", "[ArraySequence]") {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Prepend(0);

    REQUIRE(seq.GetLength() == 3);
    REQUIRE(seq.Get(0) == 0);
    REQUIRE(seq.Get(2) == 2);

    seq.InsertAt(5, 1);
    REQUIRE(seq.Get(1) == 5);
    REQUIRE(seq.Get(2) == 1);

    auto sub = seq.GetSubsequence(1, 3);
    REQUIRE(sub->GetLength() == 3);
    REQUIRE(sub->Get(0) == 5);
}

TEST_CASE("ListSequence: Operations", "[ListSequence]") {
    MutableListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Prepend(0);

    REQUIRE(seq.GetLength() == 3);
    REQUIRE(seq.Get(0) == 0);
    REQUIRE(seq.Get(2) == 2);

    seq.InsertAt(5, 1);
    REQUIRE(seq.Get(1) == 5);
    REQUIRE(seq.Get(2) == 1);

    auto sub = seq.GetSubsequence(1, 3);
    REQUIRE(sub->GetLength() == 3);
    REQUIRE(sub->Get(0) == 5);
}

TEST_CASE("Sequence: Operators", "[Sequence]") {
    MutableListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    seq1.Prepend(0);

    MutableListSequence<int> seq2;
    seq2.Append(12);
    seq2.Append(3122);
    seq2.Prepend(7);

    MutableListSequence<int> seq3 = seq1 + seq2;
    REQUIRE(seq3.GetLength() == 6);
    REQUIRE(seq3.Get(0) == 0);

    seq1.Remove(1);
    REQUIRE(seq1.Get(1) == 2);


    MutableArraySequence<int> arr1;
    arr1.Append(1);
    arr1.Append(212);

    MutableArraySequence<int> arr2;
    arr2.Append(8);

    MutableArraySequence<int> arr3 = arr1 + arr2;
    REQUIRE(arr3.GetLength() == 3);
    REQUIRE(arr3.Get(0) == 1);


    ImmutableArraySequence<std::string> imarr1;
    imarr1.Append("fd")->Append("qual");

    ImmutableArraySequence<std::string> imarr2;
    imarr2.Append("daddy")->Append("^_^");

    ImmutableArraySequence<std::string> imarr3 = imarr1 + imarr2;

    REQUIRE(imarr3.GetLast() == "^_^");
    REQUIRE(imarr2.GetFirst() == "daddy");

}
