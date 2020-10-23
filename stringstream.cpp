/*
 * Learn to use stringstream. 
 * 
 * @author Xing Lin
 * @date 2020-10-23
 */
#include <sstream>
#include <string>
#include <iostream>

using namespace std;
int main() {

    stringstream ss;
    int i=4, j=6, k=100, m=5;
    ss << i << ' ' << j  << ' ' << k << ' ' << m;
    
    string ssstr = ss.str();
    std::cout << "ss.str() = " << ssstr << std::endl;

    stringstream ss2(ssstr);
    int i2, j2, k2, m2;
    ss2 >> i2 >> j2 >> k2 >> m2;
    std::cout << "i2=" << i2 << ", j2=" << j2 << ", k2=" << k2 << ", m2=" << m2 << std::endl;
}