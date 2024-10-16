#include "MyBigInt.h"

#include <iostream>
#include <string>
#include <vector>

MyBigInt operator ""_mbi (const char* sz) {
	std::string s(sz);
	MyBigInt mbi;
	mbi.size = s.length();
	mbi.digits = new int_least8_t[mbi.size];
	for(int i = mbi.size - 1; i >= 0; i--) {
		mbi.digits[mbi.size - 1 - i] = s[i] - '0';
	}

	return mbi;
}

MyBigInt operator+(const MyBigInt& lhs, const MyBigInt& rhs) {
	MyBigInt ret;
	const MyBigInt& longerArg = (lhs.size < rhs.size) ? rhs : lhs;
	const MyBigInt& shorterArg = (lhs.size < rhs.size) ? lhs : rhs;
	ret.digits = new int_least8_t[longerArg.size + 1];
	int_least8_t reminder = 0;
	for (int i = 0; i < shorterArg.size; i++) {
		int_least8_t tempSum = longerArg.digits[i] + shorterArg.digits[i] + reminder;
		ret.digits[i] = (tempSum > 9) ? tempSum - 10 : tempSum;
		reminder = (tempSum > 9) ? 1 : 0;
	}
	for (int i = shorterArg.size; i < longerArg.size; i++) {
		int_least8_t tempSum = longerArg.digits[i] + reminder;
		ret.digits[i] = (tempSum > 9) ? tempSum - 10 : tempSum;
		reminder = (tempSum > 9) ? 1 : 0;
	}
	if (reminder == 0) {
		ret.size = longerArg.size;
	} else {
		ret.size = longerArg.size + 1;
	}
	ret.digits[longerArg.size] = reminder;

	return ret;
}

std::ostream& operator << (std::ostream& out, const MyBigInt& mbi) {
	for(int i = mbi.size - 1; i >= 0; i--) {
		out << static_cast<int>(mbi.digits[i]);
	}

	return out;
}

bool operator == (const MyBigInt& lhs, const MyBigInt& rhs) {
	if(lhs.size != rhs.size) return false;
	for(int i = 0; i < lhs.size; i++) {
		if(lhs.digits[i] != rhs.digits[i]) return false;
	}

	return true;
}

bool operator !=(const MyBigInt& lhs, const MyBigInt& rhs) {
	return !(lhs == rhs);
}


MyBigInt::MyBigInt(unsigned long long init) {
	std::string s = std::to_string(init);
	size = s.length();
	digits = new int_least8_t[size];

	for(int i = size - 1; i >= 0; i--) {
		digits[size - 1 - i] = s[i] - '0';
	}
}

MyBigInt::MyBigInt(const MyBigInt& mbi_s) {
	size = mbi_s.size;
	// ne desava se, jer jer je konstruktor
	// if(digits) delete [] digits;
	digits = new int_least8_t[size];
	for(int i = 0; i < size; i++) {
		digits[i] = mbi_s.digits[i];
	}

	// std::cout << "Calling copy\n";
}

MyBigInt::MyBigInt(MyBigInt&& mbi_s) {
	size = mbi_s.size;
	// ne desava se, jer jer je konstruktor
	// if(digits) delete [] digits;
	digits = mbi_s.digits;
	mbi_s.digits = nullptr;
	// std::cout << "Calling move\n";
}

MyBigInt& MyBigInt::operator= (const MyBigInt& mbi_s) {
	size = mbi_s.size;
	if(digits) delete [] digits;
	digits = new int_least8_t[size];
	for(int i = 0; i < size; i++) {
		digits[i] = mbi_s.digits[i];
	}

	// std::cout << "Calling copy assign\n";

	return *this;
}

MyBigInt& MyBigInt::operator= (MyBigInt&& mbi_s) {
	size = mbi_s.size;
	if(digits) delete [] digits;
	digits = mbi_s.digits;
	mbi_s.digits = nullptr;
	// std::cout << "Calling move assign\n";

	return *this;
}

MyBigInt::~MyBigInt() {
	if(digits) delete [] digits;
}

// Овде написати следећу функцију:
MyBigInt fibo(int n) {
	if(n < 2) return n;
	MyBigInt f;
	std::vector<MyBigInt> mb_vec(n + 1);
	mb_vec[0] = 0;
	mb_vec[1] = 1;
	for(int i = 2; i <= n; i++) {
		mb_vec[i] = mb_vec[i - 1] + mb_vec[i - 2];		
	}

	return mb_vec[n];
}


int main() {
	MyBigInt k = 123;
	std::cout << k << std::endl;
	k = fibo(9);
	std::cout << k << std::endl;
	k = k + 5;
	std::cout << k << std::endl;
	k = 7 + fibo(11);
	std::cout << k << std::endl;
	k = 13_mbi;
	std::cout << k << std::endl;
	MyBigInt l = fibo(300);
	if (l != 222232244629420445529739893461909967206666939096499764990979600_mbi) {
		std::cout << "Greska!" << std::endl;
		return 1;
	}
	std::cout << l << std::endl;
	{
		MyBigInt m = l;
		std::cout << m << std::endl;
	}
	std::cout << l << std::endl;
	{
		MyBigInt n = std::move(l);
		std::cout << n << std::endl;
	}
	return 0;
}
