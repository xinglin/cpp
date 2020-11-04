/*
 *
 */ 

#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <sys/time.h>

using namespace std;

class TokenBucket {
    int fillRate;
    int maxToken;

    mutex m;
    condition_variable producer;
    condition_variable consumer;
    queue<int> tokens;
    long lastfilltime;
    bool stop;

public:
    TokenBucket(int fillRate, int maxToken): fillRate(fillRate), maxToken(maxToken), lastfilltime(-1), stop(false) {}
    long gettimems() {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }

    void produce() {

        while(true) {

            unique_lock<mutex> lk(m);
            while(tokens.size() >= fillRate && stop == false)
                producer.wait(lk);

            if(stop == true)
                break;

            long now = gettimems();
            long toFill = -1;
            if(lastfilltime == -1)
                toFill = fillRate;
            else 
                toFill = (now - lastfilltime)/1000.0 * fillRate;

            std::cout << "producer: to fill " << toFill << std::endl;
            while(tokens.size() < maxToken && toFill > 0) {
                tokens.push(rand()%maxToken);
                toFill --;
            }
            if(toFill)
                lastfilltime = now;

            lk.unlock();
            consumer.notify_all();
        }

        std::cout << "producer exits\n";
    } 

    vector<int> consume(int n) {
        vector<int> res;
        
        while(n > 0) {

            unique_lock<mutex> lk(m);
            while(tokens.size() <= 0)
                consumer.wait(lk);

        
            int toFetch = ::min((int)tokens.size(), n);
            std::cout << "consumer: to fetch " << toFetch << std::endl;
            int count = toFetch;
            while(count > 0) {
                res.push_back(tokens.front());     
                tokens.pop();
                count --;
            }

            n -= toFetch;
            producer.notify_all();

        }

        return std::move(res);
    }

    void consumeToken() {

        int round = 10;
        while(round) {
            int n = rand()% 100;
            
            std::cout << "to fetch " << n << " tokens\n"; 
            vector<int> tokens = consume(n);
            for(int t: tokens) 
                std::cout << t << ",";
            std::cout << std::endl;
            round --;
        }

        unique_lock<mutex> lk(m);
        stop = true;
        producer.notify_all();
        std::cout << "consumer exits\n";
    }
};



int main() {

    TokenBucket tb(100, 5*100);

    thread producer(&TokenBucket::produce, &tb);

    thread consumer(&TokenBucket::consumeToken, &tb);

    consumer.join();
    producer.join();
}