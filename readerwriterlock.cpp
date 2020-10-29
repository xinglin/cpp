/*
 * @author Xing Lin
 * @date  2020-10-28
 * 
 * Reader writer lock. 
 * https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem#Writer
 */ 

#include "semaphore.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

mutex mcout;
mutex m;
int readerCount=0;

Semaphore rw(1);
Semaphore order(1); // use the order to ensure faireness between readers and writers. 

void acquire_readerlock() {
    order.acquire();
    unique_lock<mutex> lk(m);
    readerCount++;
    if(readerCount == 1)
        rw.acquire();
    order.release();
}

void release_readerlock() {
    unique_lock<mutex> lk(m);
    readerCount--;
    if(readerCount == 0)
        rw.release();
}

void acquire_writerlock() {
    order.acquire();
    rw.acquire();
    order.release();
}

void release_writerlock() {
    rw.release();
}

void reader_func(int id) {
    int i=0;
    while(i < 10) {
        acquire_readerlock();
        unique_lock<mutex> lk(mcout);
        std::cout << "reader " << id << " acquired reader lock\n";
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        release_readerlock();   
        i++;
    }

    unique_lock<mutex> lk(mcout);
    std::cout << "reader " << id << " exits\n";
}

void writer_func(int id) {
    int i=0;
    while(i < 10) {

        acquire_writerlock();
        unique_lock<mutex> lk(mcout);
        std::cout << "writer " << id << " acquired writer lock\n";
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        release_writerlock();
        i++;
    }
    
    unique_lock<mutex> lk(mcout);
    std::cout << "writer exits\n";
}
int main() {
    vector<thread> readers;
    thread writer (writer_func, 0);

    for(int i=0;i < 10; i++)
        readers.push_back(thread(reader_func, i));


    for(int i=0; i < 10; i++) 
        readers[i].join();

    writer.join();

}