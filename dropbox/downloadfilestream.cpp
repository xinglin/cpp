

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
struct Chunk {
    int start;
    int end;

    Chunk(int s, int e) : start(s), end(e) { }

    bool operator<(const Chunk& c2) const {
        return start < c2.start;
    }
};


class Downloader {
    vector<Chunk> array;
    int size; 
public:
    Downloader(int s): size(s) {}
    int debug=1;
    void addChunk(Chunk c) {
        if(debug)
            std::cout << "add chunk [" << c.start << " -> " << c.end << "]\n";

        auto anchor = lower_bound(array.begin(), array.end(), c);

        auto it = anchor;
        while(it != array.end() && it->start <= c.end+1) {
            c.end = it->end;
            it = array.erase(it);
        }

        anchor = lower_bound(array.begin(), array.end(), c);
        it = anchor;
        while(it != array.begin()) {
            it = ::prev(it);
            if(it->end+1 >= c.start) {
                std::cout << "merge with [" << it->start << " -> " << it->end << "]\n";
                c.start = it->start;
            }
        }

        if(debug) {
            std::cout << "latest chunk [" << c.start << " -> " << c.end  << "]" << std::endl;
            if(it != array.end())
                std::cout << "delete merged small chunks from [" << it->start << " -> " << it->end << "]\n";
            if(anchor != array.end())
                std::cout << "anchor at [" << anchor->start << " -> " << anchor->end << "]\n";
        }

        array.erase(it, anchor);

        if(debug)
            std::cout << "insert the new chunk\n";
        if(anchor != array.begin()) {
            it = ::prev(anchor);
            array.insert(it, c);
        } else {
            array.insert(array.begin(), c);
        }

        return;
    }

    bool isFileDone() {
        bool ret;
        if(array.size() > 1 || array.size()==0)
            return false;
        return array[0].start ==0 && array[0].end == size -1;
    }
};

int main () {
    Downloader d(5*100);
    Chunk c0(0, 99);
    Chunk c1(100, 199), c2(200, 299), c3(300, 399), c4(400, 499);

    
    d.addChunk(c3);
    d.addChunk(c0);
    d.addChunk(c1);
    d.addChunk(c2);
    d.addChunk(c4);
    

    bool Done = d.isFileDone();
    std::cout << "done = " << Done << std::endl;
}