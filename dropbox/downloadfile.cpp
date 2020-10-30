/*
 * Download a file in chunks. 
 */

#include <iostream>
#include <vector>
using namespace std;

struct Chunk {
    int start;
    int end;

    Chunk(int s, int e) : start(s), end(e) { }

    bool operator<(const Chunk& c2) const {
        return start < c2.start;
    }
};


bool isFileDone(vector<Chunk>& chunks, int size) {
    if(chunks.empty())
        return false;

    std::sort(chunks.begin(), chunks.end());

    std::cout << "chunk 0 = [" << chunks[0].start << " -> " << chunks[0].end << "]\n";
    int end = chunks[0].end;
    for(int i=1; i < chunks.size(); i++) {
        std::cout << "chunk i = [" << chunks[i].start << " -> " << chunks[i].end << "]\n";
        if(end + 1 < chunks[i].start)
            return false;
        end = ::max(end, chunks[i].end);
    }

    return end == size;
}

int main () {
    std::cout << "download file\n";
    vector<Chunk> v;
    for(int i=0; i < 100; i++) {
        v.emplace_back(i*100, (i+1)*100-1);
    }

    std::cout << "isFileDone returns: " << isFileDone(v, 100*100-1) << std::endl;
}
