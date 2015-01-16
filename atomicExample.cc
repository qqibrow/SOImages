#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

struct Foo {
	Foo(): x_(0) {}
	std::atomic<int> x_;
};

using namespace std;

int main() {
	Foo foo;
	thread th1(
		[&]() {
			cout << "Waiting for thread2 setting value for x" << endl;
			while (foo.x_.load(memory_order_acquire) == 0);
			int current = foo.x_.load(memory_order_acquire);
			cout << "Current value is " << current << endl;
		});
	thread th2(
		[&]() {
			std::chrono::milliseconds dura( 2000 );
            std::this_thread::sleep_for( dura );
            cout << "Setting up value for x" << endl;
            foo.x_.store(42,memory_order_release);
		});
	th1.join();
	th2.join();
	return 0;
}
