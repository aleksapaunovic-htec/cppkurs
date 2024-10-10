#ifndef _MY_BIG_INT_
#define _MY_BIG_INT_

#include <cstdint>

struct MyBigInt {
	friend MyBigInt operator+(const MyBigInt& lhs, const MyBigInt& rhs);
private:
	int_least8_t* digits = nullptr;
	int size = 0;
};

#endif
