#include "queue.h"

Queue::Queue(int maxSize){
    _maxSize = maxSize;
}

Queue::~Queue(){
    clear();
}

void Queue::push(long long val){

    if(isFull()) return;

    if(_head == nullptr){
        _head = new Node(val);
        _tail = _head;
        return;
    }

    Node* temp = new Node(val);
    _tail->next = temp;
    _tail = temp;

    _size++;
}

void Queue::pop(){
    if(isEmpty()) return;

    Node* temp = _head;
    _head = _head->next;

    if(temp == _tail) _tail = nullptr;

    delete temp;
    _size--;
}

bool Queue::isEmpty(){
    return _head == nullptr;
}

bool Queue::isFull(){
    return _size >= _maxSize;
}

int Queue::front(){
    return _head->val;
}

int Queue::back(){
    return _tail->val;
}

void Queue::clear(){
    while(_head){
        Node* toRemove = _head;
        _head = _head->next;
        delete toRemove;
    }
}