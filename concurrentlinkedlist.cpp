/*
 * Concurrent single-link list. 
 * 
 * Has a head and a tail. 
 * Supports insertion at the head and removal from the tail. 
 * Multi-thread safe. 
 * 
 */ 
#include <iostream>


template <typename T>
struct Node {
    T val;
    Node* next;

    Node() {};
    Node(T v): val(v), next(nullptr) {}
};

template <typename T>
struct LinkedList {
    Node<T> *head, *tail;
    LinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    void add(T v) {
        Node<T>* n = new Node<T> (v);
        
        if(head == nullptr) {
            head = n;
            tail = n;
        } else {
            head->next = n;
            head = n;
        }
    }

    bool empty() {
        return tail == nullptr;
    }

    bool remove(T& ret) {
        if(tail == nullptr)
            return false;

        Node<T>* n = tail;
        ret = n->val;

        tail = tail->next;
        // if we have an empty list.
        if(tail == nullptr)
            head = nullptr;

        delete n;
        return true;
    }
};

using namespace std;

int main() {

    LinkedList<int> ll;

    for(int i=0; i < 10; i++)
        ll.add(i);

    for(int i=0; i < 14; i++) {
        int val;
        if(ll.remove(val))
            cout << val << ' ';
        else
            cout << "[empty] ";
    }

    for(int i=0; i < 10; i++)
        ll.add(i);

    for(int i=0; i < 14; i++) {
        int val;
        if(ll.remove(val))
            cout << val << ' ';
        else
            cout << "remove return false" << endl;
    }
    
    return 0;
}