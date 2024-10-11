#include <iostream>
#include <cassert>

// Овде написати следећу функцију:
constexpr int fibo(int n) {
	// moze, ali svakako crkava pri kompilaciji ako je prevelik broj
	assert(n < 100);
	if(n == 0 || n == 1) return n;
	return fibo(n - 1) + fibo(n - 2);
}


int main() {
	static_assert(fibo(7) == 13, "Greska!");
	const int k = fibo(9);
	std::cout << k << std::endl;
	// constexpr int l = fibo(300); // 300. Фибоначијев број је превелик за int
}
