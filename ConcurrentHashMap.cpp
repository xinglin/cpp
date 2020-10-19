/*
 * @author Xing Lin
 * @date 2020-10-18
 * Concurrent hash map, using atomic compare_exchange().
 */
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>

using namespace std;

struct Node {
    int key;
    int val;
    atomic<Node*> next;
    
    Node(int k, int v): key(k), val(v), next(NULL) {}
};

class MyHashMap {
    
    const static int BUCKETNUM = 10000;
    atomic<Node*>  array[BUCKETNUM];
    
public:
    /** Initialize your data structure here. */
    MyHashMap() {
        for(int i=0; i < BUCKETNUM; i++)
            array[i] = nullptr;
    }
    
    // key will always be non-negative. 
    void put(int key, int value) {
        
        int bucketid = key%BUCKETNUM;
        
        Node *node = array[bucketid].load();
        while(node) {
            if (node->key == key) {
                node->val = value;
                break;
            }
            node = node->next;
        }
        
        if(!node) {
            // reach to the end
            Node* newNode = new Node(key, value), *head;
            do {
                head = array[bucketid].load();
                newNode->next.store(head);
            } while ( array[bucketid].compare_exchange_weak(head, newNode) == false);
        }
    }
    
    // Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key //
    int get(int key) {
        int bucket = key%BUCKETNUM;
        
        Node* node = array[bucket].load();
        while(node) {
            if(node->key == key)
                return node->val;
            
            node = node->next;
        }
        
        return -1;
    }
    
    // Removes the mapping of the specified value key if this map contains a mapping for the key 
    // deletion is complicated. Assume we want to delete B from A->B->C.
    // We need to both ensure A->next and B->next is not changed during 
    // our operation, which likely requries double_compare_and_exchange().
    // http://15418.courses.cs.cmu.edu/spring2013/lecture/lockfree/slide_020
    // 
    // A different approach was described in this paper, without requiring Double_compare_and_exchange. 
    //  paper: Nitro: A Fast, Scalable In-Memory Storage Engine for NoSQL Global Secondary Index
    // "We used a different approach to achieve DoubleCAS without any hardware support. 
    // Instead of next pointers, we defined a new object type for pointer next reference. 
    // The reference object holds two values, a node pointer and a isdeleted mark flag. 
    // Instead of doing CompareAndSwap on the node pointer, we perform CompareAndSwap on 
    // a pointer to this next reference object. Thus, every time the reference object is swapped, 
    // two values are atomically swapped. For a non-garbage collected language, other approaches 
    // such as tagged pointers can be used. The least 48 bits of the 64-bit virtual addresses are 
    // only used in practice. The MSB bit of the 64-bit pointer address can be used to indicate 
    // isdeleted flag. This tagging method can avoid the extra memory overhead of pointer reference object."
    void remove(int key) {
        
    }
    
};

MyHashMap map;

void thread_func(int id) {
    std::cout << "thread " << id << " starts\n";
    for(int i=0; i < 10000; i++) {
        int key = id*10000 + i, value = key;
        map.put(key, value);
        int fetched_value = map.get(key);
        assert(fetched_value == value);
    }
}

int main() {
    std::cout << "hello concurrent hash map." << std::endl;

    vector<thread> threads;
    for(int i=0; i < 10; i++)
        threads.push_back(::thread(thread_func, i));

    for(auto& t: threads)
        t.join();    
}