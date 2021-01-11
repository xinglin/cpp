/*
 * @author Xing Lin
 * @date 2020-10-15
 * 
 * A sample program, to demonstrate how to build a three-stage data processing pipeline, 
 * using mutex and condition variables. Each stage could have multiple threads for concurrent
 * processing. 
 * 
 * Data (chunks) flows from file_read_thread to compression threads to encryption threads.
 */ 
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

const int COMPRESS_THREAD_NUM=10;
const int ENCRYPT_THREAD_NUM=10;
const int CHUNK_NUM = 10000;

struct Chunk {
    int id;
    int size;
    int checksum;
    char* buf;
    Chunk(int id, int s): id(id), size(s), checksum(0), buf(NULL) {}
};

// mutex/cv for communication between file read thread and compression thread.
queue<Chunk*> cq;   // compression queue
::mutex m;
::condition_variable cv_producer;
::condition_variable cv_consumer;
bool file_read_thread_done = false;


void file_read_thread(){
    std::cout << "file_read_thread starts." << std::endl;
    for(int i=0; i < CHUNK_NUM; i++) {

        unique_lock<mutex> lk(m);
        while(cq.size() >= 10)
            cv_producer.wait(lk);
        
        cq.push(new Chunk(i, 1020));
        std::cout << "file_read_thread produces chunk " << i << std::endl;
        
        cv_consumer.notify_one();
    
        //lk.unlock();  // not really need, as it will be unlock automatically when the unique_lock is destroyed
    }
 
    std::cout << "file_read_thread exits." << std::endl;
}

// mutex/cv for communication between compression thread and encryption thread.
queue<Chunk *> eq;  // encryption queue
mutex m2;
condition_variable cv_compress;
condition_variable cv_encrypt;
bool compress_thread_done = false;


void compress_thread(int i)
{
    std::cout << "compress_thread " << i << " starts." << std::endl;
    while(true) {


        unique_lock<mutex> lk(m);
        while(cq.size()==0 && file_read_thread_done == false)
            cv_consumer.wait(lk);

        if(cq.size()==0 && file_read_thread_done) {
            //lk.unlock();
            break;
        }
        
        Chunk* c = cq.front();
        cq.pop();
        std::cout << "compress_thread " << i << " compress chunk " << c->id << std::endl;
        lk.unlock();
        cv_producer.notify_one();


        // push for encryption
        unique_lock<mutex> lk2(m2);
        while(eq.size() >=10)
            cv_compress.wait(lk2);

        eq.push(c);
        cv_encrypt.notify_one();
        lk2.unlock();

    }
    std::cout << "compress_thread " << i << " exists." << std::endl;
}


void encrypt_thread(int id) {
    std::cout << "encrypt_thread " << id << " starts." << std::endl;
    while(true) {

        unique_lock<mutex> lk(m2);
        while(eq.size()==0 && compress_thread_done==false)
            cv_encrypt.wait(lk);

        if(eq.size()==0 && compress_thread_done)
            break;

        Chunk* c = eq.front();
        eq.pop();
        std::cout << "encrypt_thread " << id << " encrypt-chunk " << c->id << std::endl;
        lk.unlock();
        cv_compress.notify_one();
    }
    std::cout << "encrypt_thread " << id << " exists." << std::endl;
}

int main() {

    std::thread file_read = std::thread(file_read_thread);
    vector<thread> compress_threads, encrypt_threads;
    for(int i = 0; i < COMPRESS_THREAD_NUM; i++) {
        compress_threads.push_back(::thread(compress_thread, i));
    }

    for(int i=0; i < ENCRYPT_THREAD_NUM; i++)
        encrypt_threads.push_back(::thread(encrypt_thread, i));

    // wait for all thread to join.
    file_read.join();

       
    unique_lock<mutex> lk(m);
    file_read_thread_done = true;
    lk.unlock();
    cv_consumer.notify_all();       // wake up compress threads to exit.

    for(auto& t: compress_threads)
        t.join();
    
    unique_lock<mutex> lk2(m2);
    compress_thread_done = true;
    lk2.unlock();
    cv_encrypt.notify_all();


    for(auto& t: encrypt_threads)
        t.join();
    
    std::cout << "main thread exits." << std::endl;
}