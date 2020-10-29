/*
 * Learn to use stringstream. 
 * 
 * @author Xing Lin
 * @date 2020-10-23
 */
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
int main() {

    stringstream ss;
    int i=4, j=6, k=100, m=5;
    ss << i << ' ' << j  << ' ' << k << ' ' << m;
    
    std::cout << "hello world\n";
     
    string ssstr = ss.str();
    std::cout << "ss.str() = " << ssstr << std::endl;

    stringstream ss2(ssstr);
    int i2, j2, k2, m2;
    ss2 >> i2 >> j2 >> k2 >> m2;
    std::cout << "i2=" << i2 << ", j2=" << j2 << ", k2=" << k2 << ", m2=" << m2 << std::endl;

    stringstream ss3(ssstr);
    vector<int> array;
    int v;
    while(!ss3.eof()) {
        
        ss3 >> v;
        array.push_back(v);
    }

    for(auto i: array) 
        std::cout << i << ' ';
    std::cout << std::endl;
}