/*
 * @author Xing Lin
 * @date 2020-10-28
 * 
 * Implement semaphore using mutex and condition variable.
 */ 
#include <mutex>
#include <condition_variable>
#include <iostream>

struct Semaphore {
    std::mutex m;
    std::condition_variable cv;    
    int count;

    Semaphore(int c): count(c) {}

    void acquire() {
        std::unique_lock<std::mutex> lk(m);
        while(count <= 0)
            cv.wait(lk);
        count --;    
    }

    void release() {
        std::unique_lock<std::mutex> lk(m);
        count++;
        cv.notify_one();
    }
};