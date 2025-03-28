#include "linked_list.hpp"

/*
LinkedList<T>::метод(){}
*/

template <class T>
LinkedList<T>::LinkedList(){
    root = nullptr;
    size = 0;
}

template <class T>
LinkedList<T>::LinkedList(T* items, int count){
    if (count < 0){
        throw std::invalid_argument("Negative_count");
    }

    if (count == 0) {
        root = nullptr;
        size = 0;
        return;
    }

    size = count;
    root = new Node{items[0], nullptr};
    Node* current = root;

    for(int i = 1; i<count; i++){
        Node* newNode =  new Node{items[i], nullptr};
        current->next = newNode;
        current = newNode;
    }
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list){
    if (list.root == nullptr) {
        root = nullptr;
        size = 0;
        return;
    }

    root = new Node{list.root->data, nullptr};
    Node* currentThis = root;
    Node* currentOther = list.root->next;

    while (currentOther != nullptr) {
        currentThis->next = new Node{currentOther->data, nullptr};
        currentThis = currentThis->next;
        currentOther = currentOther->next;
    }

    size = list.size;
}

template <class T>
LinkedList<T>::~LinkedList(){
    Node* cur = root;
    while (cur != nullptr) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }    
}

template <class T>
T LinkedList<T>::GetFirst() const{
    if(root == nullptr){
        throw std::range_error("The LinkedList is empty");
    }

    return root->data;
}

template <class T>
T LinkedList<T>::GetLast() const{
    if(root == nullptr){
        throw std::range_error("The LinkedList is empty");
    }

    Node* cur = root;

    while (cur->next != nullptr){
        cur = cur->next;
    }

    return cur->data;
}

template <class T>
T LinkedList<T>::Get(int index) const{
    if(root == nullptr){
        throw std::range_error("The LinkedList is empty");
    }

    if(index < 0 || index >= size){
        throw std::out_of_range("index out of range");
    }

    if(index == 0){
        return root->data;
    }

    Node* cur = root;
    for(int i = 0; i<index; i++){
        cur = cur->next;
    }
    return cur->data;
}

template <class T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw std::out_of_range("Invalid start or end index");

    LinkedList<T>* sublist = new LinkedList<T>();
    Node* current = root;

    for (int i = 0; i < startIndex; i++) {
        current = current->next;
    }

    sublist->Append(current->data);

    for (int i = startIndex + 1; i <= endIndex; i++) {
        current = current->next;
        sublist->Append(current->data);
    }

    return sublist;
}


template <class T>
int LinkedList<T>::GetLength() const{
    return size;
}

template <class T>
void LinkedList<T>::Append(T item){
   
    Node* newNode = new Node{item, nullptr};  
    if(root == nullptr){
        root = newNode;
    }else{
        Node* cur = root;
        while (cur->next != nullptr)
        {
            cur = cur->next;
        }
        cur->next = newNode;
    }
    size++;
}

template <class T>
void LinkedList<T>::Prepend(T item){
    Node* newNode = new Node{item, root};
    root = newNode;
    size++;
}

template <class T>
void LinkedList<T>::InsertAt(T item, int index){
    if(index>size || index<0){
        throw std::out_of_range("Index out of range");
    }
    Node* cur = root;
    Node* newNode = new Node{item};
    if(index == 0){
        newNode->next = root;
        root = newNode;
    }else{
        for(int i=0; i<index; i++){
            cur = cur->next;
        }
        Node* tmp = cur->next;
        cur->next = newNode;
        newNode->next = tmp;
    }
    size++;
}

template <class T>
void LinkedList<T>::Remove(int index){
    if(size == 0) return;
    
    if(index<0 || index>=size) throw std::out_of_range("Index out of range");

    Node* cur = root;
    Node* tmp;

    if(index == 0){
        root = cur->next;
        delete(cur);
    }else{
        for(int i=0; i<index-1; i++){
            cur = cur->next;
        }
        tmp = cur->next;
        cur->next=tmp->next;
        delete(tmp);
    }
    size--;
}

template <class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list){
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* cur = list->root;
    while (cur != nullptr){
        result->Append(cur->data);
        cur = cur->next;
    }
    return result;  
}