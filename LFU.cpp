/* 
 * LFU cache.
 * leetcode #460.
 */ 

struct Node {
    int key;
    int val;
    int c;      // access count    
    Node* prev;
    Node* next;
    Node(int k, int v): key(k), val(v), c(1), prev(nullptr), next(nullptr) {}
};

struct DLList {
    Node* head, *tail;
    int size = 0;
    DLList() {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }
    
    void insertNode(Node* n) {
        Node* next = head->next;
        
        n->next = next;
        next->prev = n;
        
        head->next = n;
        n->prev = head;
        ++size;
    }
    
    void removeNode(Node* n) {
        // cout << "  remove key=" << n->key << ", c = " << n->c << endl;
        
        Node* prev = n->prev;
        Node* next = n->next;
        
        prev->next = next;
        next->prev = prev;
        
        // reset next and prev pointers for the current node to null.
        n->next = nullptr;
        n->prev = nullptr;
        
        --size;
    }
    
    bool empty() {
        return size == 0;
    }
};

class LFUCache {
    map<int, DLList*> LFUmap;
    unordered_map<int, Node*> kvmap;
    int count;
    int capacity;
    
    void initializeLFUList(int c) {
        DLList *list = new DLList();
        LFUmap[c] = list;
    }
public:
    LFUCache(int capacity): capacity(capacity), count(0) {
        
    }
    
    int get(int key) {
        if(capacity==0)
            return -1;
        
        //cout << "get " << key << endl;
        
        if(kvmap.find(key) == kvmap.end()) {
            //cout << " Not found, get return -1" << endl;
            return -1;
        }
        
        Node* n = kvmap[key];
        int& c = n->c;      // use c for convenience
        DLList *list = LFUmap[c];
        // remove it from the current double-link list.
        list->removeNode(n);
        if(list->empty())
            LFUmap.erase(c);
        
        c++;    //increase the access 
        
        // insert into the new LRU list
        if(LFUmap.find(c) == LFUmap.end()) {
            // when LFU list for c does not exist. Initialize one.
            initializeLFUList(c);
        }
        
        DLList *newList = LFUmap[c];
        newList->insertNode(n);
        
        //print();
        
        return n->val;
    }
    
    /*void print() {
        cout << "  print cache status: " << endl;
        for(auto it = LFUmap.begin(); it != LFUmap.end(); it++) {
            int c = it->first;
            Node* h = it->second;
            cout << "   c= " << c << ", keys: ";
            Node* d = h->next;
            while(d->next) {
                cout << d->key << ", ";
                d = d->next;
            }
            cout << " head->c=" << h->c << ", tail->c=" << d->c << endl;
            //cout << endl;
        }
        cout << "   cache status done" << endl;
    }*/
    void evict() {
        
        auto it = LFUmap.begin();
        DLList* list = it->second;
        Node* last = list->tail->prev;
        list->removeNode(last);
        if(list->empty()) 
            LFUmap.erase(it->first);
        
        kvmap.erase(last->key);
        delete last;
        
        //print();
    }
    void put(int key, int value) {
        if(capacity==0)
            return;
        
        //cout << "put " << key << endl;
        Node* n = kvmap[key];
        if(!n) {
            n = new Node(key, value);
            kvmap[key] = n;
            
            ++count;
            
            if(count == capacity+1) {
                --count;
                evict();
            }
        } else {
            n->val = value;
            DLList *list = LFUmap[n->c];
            list->removeNode(n);
            if(list->empty())
                LFUmap.erase(n->c);
            
            n->c++;
        }
        
        int c = n->c;
        if(LFUmap.find(c) == LFUmap.end())
            initializeLFUList(c);
        
        DLList* newList = LFUmap[c];
        newList->insertNode(n);
        //print();
    }
};

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */