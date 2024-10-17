#ifndef _MY_BIG_INT_
#define _MY_BIG_INT_

#include <cstdint>
#include <iostream>
#include <memory>

struct MyBigInt {
	friend MyBigInt operator+(const MyBigInt& lhs, const MyBigInt& rhs);
	friend MyBigInt operator ""_mbi (const char* sz);
	friend bool operator != (const MyBigInt& lhs, const MyBigInt& rhs);
	friend bool operator == (const MyBigInt& lhs, const MyBigInt& rhs);
	friend std::ostream& operator << (std::ostream& out, const MyBigInt& mbi);
	//ctors
	MyBigInt(unsigned long long init);
	MyBigInt() = default;
	MyBigInt(const MyBigInt& mbi_s);
	MyBigInt(MyBigInt&& mbi_s);
	MyBigInt& operator=(const MyBigInt& mbi_s);
	MyBigInt& operator=(MyBigInt&& mbi_s);
	~MyBigInt();

private:
	std::unique_ptr<int_least8_t[]> digits;
	int size = 0;
};

#endif
