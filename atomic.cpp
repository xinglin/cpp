/*
 * @author Xing Lin
 * @date 2020-10-16
 * 
 * play with atomic types
 */

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

using namespace std;

atomic<int> c(0);
int v(0);


void thread_fun(int id) {
    for(int i=0; i < 10000; i++) {
        c.fetch_add(1);
        v++;
    }
}

int main() {
    std::cout << "hello atomic types" << std::endl;

    ::vector<thread> threads;
    for(int i=0; i < 10; i++) 
        threads.push_back(::thread(thread_fun, i));

    for(auto & t: threads)
        t.join();

    std::cout << "main thread gets c=" << c 
        << ", v=" << v << std::endl;
}

