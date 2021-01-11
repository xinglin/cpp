/*
 * Concurrent single-link list. 
 * 
 * Has a head and a tail. 
 * Supports insertion at the head and removal from the tail. 
 * Multi-thread safe. 
 * 
 */ 
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;


template <typename T>
struct Node {
    T val;
    Node* next;

    Node() {};
    Node(T v): val(v), next(nullptr) {}
};

atomic<bool> complete {false};

template <typename T>
struct LinkedList {
    Node<T> *head, *tail;
    mutex mh, mt;
    condition_variable cv;
    LinkedList() {
        head = new Node<T>();
        tail = head;
    }

    void add(T v) {
        Node<T>* n = new Node<T> (v);
        if(n == nullptr) {
            cerr << "new error in add() " << endl;
            return;
        }

        unique_lock<mutex> lk(mh);
        //unique_lock<mutex> lk2(mt);
        head->next = n;
        head = n;
        //lk.unlock();
        //cv.notify_one();     
    }

    bool empty() {
        unique_lock<mutex> lk(mh);
        return tail == head;
    }

    bool remove(T& ret) {
        unique_lock<mutex> lk(mh);
        //unique_lock<mutex> lk2(mt);
        
        //while(tail == head && complete == false)
        //    cv.wait(lk);

        if(tail == head)
            return false;

        Node<T>* n = tail->next;
        if( n == nullptr ) {
            cerr << "n is nullptr" << endl;
            exit(1);
        }

        ret = n->val;

        Node<T>* prev = tail;
        tail = tail->next;
        delete prev;
        
        return true;
    }
};

LinkedList<int> ll;
const int N = 1000000;
void addlist(int i) {
    cout << "producer " << i << " starts" << endl;
    for(int i=0; i < N; i++)
        ll.add(i);
    cout << "producer " << i << " finishes " << endl;
}


void removelist(int i) {
    cout << "consumer " << i << " starts " << endl;
    int val = -1;
    while( ll.remove(val) ); 
        
    cout << "consumer " << i << " last fetched " << val << endl;
    
}

int main() {

    
    /*
    vector<thread> producers, consumers;
    
    for(int i=0; i < 1; i++) {
        producers.push_back(thread(addlist, i));
        consumers.push_back(thread(removelist, i));
    }

    for(int i=0; i < 1; i++) {
        producers[i].join();
    }
    cout << "producers complete " << endl;

    complete = true;

    for(int i=0; i < 1; i++) {
        consumers[i].join();
    }

    cout << "consumers complete " << endl;
    */


   addlist(0);
   removelist(0);

   addlist(0);
   removelist(0);
    return 0;
}