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

    long gettimems() {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }

long starttime;

class TokenBucket {
    int fillRate;
    int maxToken;

    mutex m;
    condition_variable producer;
    condition_variable consumer;
    queue<int> tokens;
    long lastfilltime;
    bool stop;
    long sum = 0;

public:
    TokenBucket(int fillRate, int maxToken): fillRate(fillRate), maxToken(maxToken), lastfilltime(-1), stop(false) {}


    void produce() {

        while(true) {

            unique_lock<mutex> lk(m);
            while(tokens.size() >= maxToken && stop == false)
                producer.wait(lk);

            if(stop == true)
                break;

            long now = gettimems();
            long toFill = -1;
            if(lastfilltime == -1)
                toFill = maxToken;
            else 
                toFill = (now - lastfilltime)/1000.0 * fillRate;

            toFill = ::min((int)toFill, int(maxToken-tokens.size()));
            
            

            int count = toFill;
            while(count > 0) {
                tokens.push(rand()%maxToken);
                count --;
            }

            if(toFill) {
                sum += toFill;
                lastfilltime = now;
                std::cout << "now=" << (now-starttime)/1000 
                    << ", total token produced: " << sum 
                    << ", available tokens=" << tokens.size() << std::endl;
            }
            
            lk.unlock();
            consumer.notify_all();
            //std::this_thread::yield();
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
            //std::cout << "consumer: to fetch " << toFetch << ", available token=" << tokens.size() << std::endl;
            int count = toFetch;
            while(count > 0) {
                res.push_back(tokens.front());     
                tokens.pop();
                count --;
            }

            n -= toFetch;
            lk.unlock();
            producer.notify_all();
        }

        return std::move(res);
    }

    void consumeToken() {
        std::srand(std::time(0));

        int round = 100;
        while(round) {
            int n = rand()% 1000;
            
            std::cout << "consumer to fetch " << n << " tokens\n"; 
            vector<int> tokens = consume(n);
            //for(int t: tokens) 
            //    std::cout << t << ",";
            std::cout << std::endl;
            round --;
            std::this_thread::sleep_for(chrono::seconds(1));
        }

        unique_lock<mutex> lk(m);
        stop = true;
        producer.notify_all();
        std::cout << "consumer exits\n";
    }
};



int main() {
    starttime = gettimems();

    TokenBucket tb(100, 4*100);

    thread producer(&TokenBucket::produce, &tb);

    thread consumer(&TokenBucket::consumeToken, &tb);

    consumer.join();
    producer.join();
}