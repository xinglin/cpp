#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include <condition_variable>


class Test {
   int x;
   int y;
};

using namespace std;

std::mutex m;
condition_variable cv;
int turn=0;


int thread_function(int i) {
	unique_lock<mutex> lk(m);
	while(turn != i) {
		cv.wait(lk);
		std::cout << "thread " << i << " is woke up" << std::endl;
	}
	std::cout << "thread " << i << " is running" << std::endl;
	turn++;
	// have to use notify_all() because notify_one() will not gurantee to notify the next thread.
	cv.notify_all(); 
}

int main() {
	std::cout << "hello world" << std::endl;
	vector<thread> v;
	for(int i=0; i < 10; i++)
		v.emplace_back(thread_function, i);
	
	for(thread& t: v)
		t.join();
}
