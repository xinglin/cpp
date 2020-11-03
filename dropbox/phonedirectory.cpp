/*
 * leetcode 379 - phone directory.
 */ 
#include <vector>
#include <iostream>

using namespace std;

/*
 * bitset contains 2 * maxNumber bits. The first maxNumber bits are used as indexing
 * while the second maxNumber bits represents whether a number is available or not.
 * We set all bits to false, to indicate they are not allocated (and available).  
 * 
 * The 0th element in bitset is not used. 
 * root of the segment tree starts from 1st elment. 
 * left child = root * 2;
 * right child = root * 2 + 1;
 */
class PhoneDirectory {
    vector<bool> bitset;
    int maxNumbers;
public:
    /** Initialize your data structure here
        @param maxNumbers - The maximum numbers that can be stored in the phone directory. */
    PhoneDirectory(int maxNumbers): maxNumbers(maxNumbers) {
        bitset.resize(maxNumbers*2, false);
    }
    
    /*
     * find the next available number, starting from root (e.g., findNext(1) ).
     * bitset[1] should be false (there are numbers still available), 
     * before calling findNext();
     * 
     * If we have reached the number area ( [maxNumber, 2*maxNumber) ) and it is 
     * available, return  
     */
    int findNext(int root) {

        if(root >= maxNumbers && root < 2*maxNumbers && bitset[root] == false)
            return root-maxNumbers;

        if(bitset[2*root] == false)
            return findNext(2*root);    
        else if(bitset[2*root+1] == false)
            return findNext(2*root+1);
        else 
            return -1;
    }

    /*
     * update parent nodes when number id is allocated.
     * If the current parent no longer has available numbers, keep updating its parents. 
     * Otherwise, stop. 
     */ 
    void updateIndexSet(int id) {

        int parent = (id+maxNumbers)/2;
        while(parent >= 1) {

            bitset[parent] = (bitset[parent*2] == true && bitset[parent*2+1] == true);
            if(bitset[parent] == false)
                break;
            
            parent = parent / 2;
        }
    }

    /** Provide a number which is not assigned to anyone.
        @return - Return an available number. Return -1 if none is available. */
    int get() {
        // check the root first.
        // if root is set to true, it means all numbers are allocated.
        if(bitset[1])
            return -1;

        int id = findNext(1);
        bitset[id+maxNumbers] = true;
        updateIndexSet(id);
        return id;
    }
    
    /** Check if a number is available or not. */
    bool check(int number) {
        return bitset[maxNumbers+number] == false;
    }
    
    /*
     * Update parent nodes to indicate a new number is available in this sub-tree.
     * If the parent node is already set as available (the other branch also has available numbers),
     * no need to update further.
     */ 
    void updateIndexClear(int id) {
        int parent = (id+maxNumbers)/2;
        while(parent >= 1) {
            if(bitset[parent] == false)
                break;

            bitset[parent] = false;
            parent = parent / 2;
        }
    }

    /** Recycle or release a number. */
    void release(int number) {
        if(number < 0 || number >= maxNumbers)
            return;

        bitset[number + maxNumbers] = false;
        updateIndexClear(number);
    }
};

int main() {

    PhoneDirectory dir(3);
    int p1 = dir.get();         // return 1
    int p2 = dir.get();         // return 2

    bool c1  = dir.check(0);    // 0 is still available

    int p3 = dir.get();         // 0 is allocated.

    bool c2 = dir.check(0);     // 0 is not available.

    dir.release(0);

    bool c3 = dir.check(0);

    std::cout << "p1 = " << p1 << ", p2 = " << p2 
        << ", c1=" << c1 << ", p3= " << p3 <<
        ", c2=" << c2 << ", c3=" << c3 << std::endl; 

    return 0;
}