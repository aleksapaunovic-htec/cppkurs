#include <iostream>
#include <thread>
#include <chrono>

void f1() {
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(3s);
	std::cout << 1 << std::endl;
}

void f2(std::thread& t1) {
	t1.join();
	std:: cout << 2 << std::endl;
}

void f3(std::thread& t2) {
	t2.join();
	std:: cout << 3 << std::endl;
}


int main() {
	std::thread t1(f1);
	std::thread t2(f2, std::ref(t1));
	std::thread t3(f3, std::ref(t2));

	t3.join();


	return 0;
}
