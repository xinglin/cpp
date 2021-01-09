#include <iostream>

template <typename T>
T myMax(T i, T j) {
    return (i > j) ? i : j;
}
using namespace std;


template <typename T>
class MyVector {
    T* v;
    int size;
    int index;
public:
    MyVector(int s): size(s), index(0)  {
        v = new T[s];
    }
    void add(T n) {
        v[index] = n;
        index = (index+1)%size;
    } 

    friend ostream& operator<<(ostream& out, const MyVector& myv) {
        for(int i=0; i < myv.index; i++)
            out << myv.v[i] << ' ';
        return out;
    }
};


int main() {

    cout << myMax<int>(1, 4) << endl;
    cout << myMax<double>(1.0, 3.0) << endl;

    MyVector<int> myv(10);
    myv.add(1);
    myv.add(2);
    myv.add(90);
    cout << myv << endl;

    return 0;
}