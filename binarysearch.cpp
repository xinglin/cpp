


#include <iostream>
#include <vector>
using namespace std;
// find the next element larger or equal to t
int ceilsearch(vector<int>& v, int t) {
    int s = 0, e = v.size()-1;
    int ans=-1;
    while(s <= e) {
        int mid = (s+e)/2;
        if(v[mid] >= t) {
            ans = mid;
            e = mid - 1;
        } else {
           // v[mid] < t
           s = mid + 1;     
        }
    }

    return v[ans];
}

// find the first element smaller/equal to t
int floorsearch(vector<int>& v, int t) {
    int s = 0, e = v.size()-1;
    int ans=-1;
    while(s <= e) {
        int mid = (s+e)/2;
        if(v[mid] > t) {
            e = mid - 1;
        } else {
           // v[mid] <= t
           ans = mid;
           s = mid + 1;     
        }
    }

    return v[ans];
}

int main() {

    vector<int> v{1,2,3,5,6,8,9,10};

    int ceil = ceilsearch(v, 5);
    int floor = floorsearch(v, 8);
    std::cout << "ceil for 5 = " << ceil << ", floor for 8 = " << floor << std::endl; 
}