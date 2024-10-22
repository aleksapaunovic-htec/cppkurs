#include <thread>
#include <iostream>
#include <vector>
#include <semaphore>
#include <atomic>
#include <mutex>
#include <chrono>
#include <condition_variable>

constexpr int RING_SIZE = 8;
std::vector<char> vec_in(RING_SIZE, 0), vec_out(RING_SIZE, 0);
// std::atomic<int> free_in{RING_SIZE};
int free_in = RING_SIZE, free_out = RING_SIZE;
// dva mutexa, jedan za ulazni, jedan za izlazni niz
std::mutex entry, ex;

std::condition_variable empty_c_in, full_c_in, empty_c_out, full_c_out;

void produce() {
	int write = 0;
	while(1) {
		char c;
		std::cin >> c;

		std::unique_lock<std::mutex> lock(entry);
		full_c_in.wait(lock, [&]() { return free_in > 0; });

		vec_in[write] = c;
		write = (write + 1) % RING_SIZE;
		free_in--;

		empty_c_in.notify_all();	
	}
}

void modify() {
	int read = 0;
	int write = 0;
	while(1) {
		char c;
		{
		       	std::unique_lock<std::mutex> lock(entry);
			empty_c_in.wait(lock, [&]() { return free_in < RING_SIZE; });
			
			c = std::toupper(vec_in[read]);

			read = (read + 1) % RING_SIZE;
			free_in++;

			full_c_in.notify_all();
		}
		{
			std::unique_lock<std::mutex> lock(ex);
			full_c_out.wait(lock, [&]() { return free_out > 0; });
			
			vec_out[write] = c;
			write = (write + 1) % RING_SIZE;
			free_out--;
				
			empty_c_out.notify_all();								
		}
	}
}

void consume() {
	int read = 0;
	while(1) {
		{
			std::unique_lock<std::mutex> lock(ex);	
			empty_c_out.wait(lock, [&]() { return free_out < RING_SIZE; });
			
			char c = vec_out[read];	
			std::cout << c << std::flush;

			read = (read + 1) % RING_SIZE;
			free_out++;

			full_c_out.notify_all();
		}
	}
}

int main() {
	std::thread t1(produce);
	std::thread t2(modify);
	std::thread t3(consume);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
