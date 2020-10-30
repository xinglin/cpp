/*
 * detect top K elements for a stream.
 */ 

#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>

using namespace std;

struct Photo {
    int id;
    int count;
    Photo(int id, int count) : id(id), count(count) {}
    bool operator<(const Photo& p2) const {
        return count < p2.count || count == p2.count && id < p2.id;
    }
    bool operator==(const Photo& p2) const {
        return id == p2.id;
    }
};

class PhotoViewTracker {
    unordered_map<int, int> countmap;  // photoid -> count
    set<Photo> set;
    int k;
public:
    PhotoViewTracker(int k):k(k) {}

    void view(int photoid) {
        
        Photo p (photoid, countmap[photoid]);
        countmap[photoid]++;
        if(set.find(p) != set.end()) {
            set.erase(p);
            p.count++;
            set.insert(p);
        } else {
            p.count++;
            if(set.size() < k)
                set.insert(p);
            else if(set.begin()->count < p.count) {
                set.insert(p);
                set.erase(set.begin());
            }
        }
    }

    vector<int> getTopK() {
        vector<int> v;
        for(auto it = set.begin(); it != set.end(); it++) {
            v.push_back(it->id);
        }

        return std::move(v);
    }
};


class PhotoViewTracker2 {
    unordered_map<int, shared_ptr<Photo>> countmap;  // photoid -> count
    set<shared_ptr<Photo>> set;
    int k;
public:
    PhotoViewTracker2(int k):k(k) {}

    void view(int photoid) {
        
        shared_ptr<Photo> p = countmap[photoid];
        if(p && set.find(p) != set.end()) {
            set.erase(p);
            p->count++;
            set.insert(p);
        } else {
            
            if(!p) {
                p = make_shared<Photo>(photoid, 1);
                countmap[photoid] = p;
            } else 
                p->count ++;

            if(set.size() < k)
                set.insert(p);
            else if((*set.begin())->count < p->count) {
                set.insert(p);
                set.erase(set.begin());
            }
        }
    }

    vector<int> getTopK() {
        vector<int> v;
        for(auto it = set.begin(); it != set.end(); it++) {
            v.push_back((*it)->id);
        }

        return std::move(v);
    }
};

int main() {
    PhotoViewTracker2 t(2);
    t.view(1);
    t.view(2);

    t.view(5);t.view(5);
    t.view(5);t.view(5);
    vector<int> v = t.getTopK();
    for(auto i: v) {
        std::cout << i << std::endl;
    }
}
