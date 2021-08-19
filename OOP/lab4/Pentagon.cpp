#include <iostream>
#include <cmath>
#include "Pentagon.h"


pentagon::pentagon() : pentagon(0)
{
}

pentagon::pentagon(size_t a)
{
	if (a <= 0) {
		std::cout << "Incorrect size" << std::endl;
	}
	side_a = a;
}

pentagon::pentagon(const pentagon& orig)
{
	side_a = orig.side_a;
}

int pentagon::F() {  /////
	return 3;      /////
}

double pentagon::Square()
{
	return side_a * side_a * 0.25 * sqrt(25 + 10 * sqrt(10));
}

void pentagon::Print()
{
	std::cout << "a = " << side_a << std::endl;
}

std::ostream& operator<<(std::ostream& os, const pentagon& right)
{
	os << "Pentagon with side a = " << right.side_a << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, pentagon& right)
{
	is >> right.side_a;
	while (right.side_a <= 0) {
		std::cout << "Incorrect data. Please repeat enter:" << std::endl;
		is >> right.side_a;
	}
	return is;
}

bool operator==(const pentagon& left, const pentagon& right)
{
	if (left.side_a == right.side_a) {
		return true;
	}
	return false;
}

pentagon& pentagon::operator=(const pentagon& right)
{
	side_a = right.side_a;
	return *this;
}

pentagon::~pentagon()
{
}
