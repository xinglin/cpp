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
string data = "";
bool ready=false;
bool processed=false;

int thread_function(int i) {
	::unique_lock<mutex> lk(m);
	while(ready != true)
		cv.wait(lk);
	std::cout << "child thread " << i << " is running" << std::endl;
	data += " + child" + to_string(i);

	processed = true;
	lk.unlock();
	cv.notify_one();
}

int main() {
	std::cout << "hello world" << std::endl;

	unique_lock<mutex> lk(m);
	data += " + parent ";
	ready = true;
	lk.unlock();
	cv.notify_one();

	thread t(thread_function, 1);
	lk.lock();
	while(processed != true)
		cv.wait(lk);

	t.join();
	std::cout << "data=" << data << std::endl;
}
