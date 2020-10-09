#include<iostream>
#include<thread>
#include<vector>
#include<mutex>

class Test {
   int x;
   int y;
};

using namespace std;

std::mutex m;

int thread_function(int i) {
	m.lock();
	std::cout << "thread " << i << " is running" << std::endl;
	m.unlock();
}

int main() {
	std::cout << "hello world" << std::endl;

	vector<thread> v;
	for(int i=0; i < 10; i++)
		v.emplace_back(thread_function, i);
	
	for(thread& t: v)
		t.join();
}
