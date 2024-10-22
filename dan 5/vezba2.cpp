#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std::chrono_literals;

std::mutex m1, m2;

// moguce je i u nitima uraditi inicijalizaciju, koriscenjem npr. dodatnog semafora na pocetku 
int a = 1, b = 2;

void f1() {
	std::vector<std::pair<int, int>> values{
		{1, 2},
		{3, 4},
		{5, 6}
	};
	int cnt = 0;
	while(true) {
		{
			std::scoped_lock(m1, m2);
			a = values[cnt].first;	
			b = values[cnt].second;

			cnt = (cnt + 1) % 3;
		}	
		int sleep  = (std::rand() % 800) + 100;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}

void f2() {
	while(true) {
		{
			std::scoped_lock(m1, m2);
			int sum = a + b;
			std::cout << sum << std::endl;
		}
		int sleep  = (std::rand() % 800) + 100;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
}


int main() {
	std::thread t1(f1);
	std::thread t2(f2);

	t2.join();
	t1.join();
	return 0;
}
