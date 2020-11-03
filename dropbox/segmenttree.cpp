#include <iostream>
#include <vector> 

using namespace std;

void buildTree(vector<int>& tree, const vector<int>& array, int root, int start, int end) 
{
    if(start == end ){
        //tree[root] = array[start];
        return;
    } else {

        int mid = (start+end)/2;
        buildTree(tree, array, 2*root, start, mid);
        buildTree(tree, array, 2*root+1, mid+1, end);
        tree[root] = tree[2*root] + tree[2*root+1];
    }
}

vector<int> buildSegmentTree(const vector<int>& array)
{
    vector<int> tree(array.size()*2);
    for(int i=array.size(); i < tree.size(); i++) 
        tree[i] = array[i-array.size()];


    for(int i = array.size()-1; i >=1; i--) {
        tree[i] = tree[i*2] + tree[i*2+1];
    }
    
    return std::move(tree);
}
int main() {

    vector<int> array = {1,2,3,4,5,6,7};;

    vector<int> tree = buildSegmentTree(array);

    for(int i=0; i < tree.size(); i++) {
        std::cout << i << ",\t";
    }
    std::cout << std::endl;
    for(int i=0; i < tree.size(); i++) {
        std::cout << tree[i] << ",\t";
    }
    std::cout << std::endl;
}





