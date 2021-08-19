#include <iostream>
#include <cmath>
#include "Rhombus.h"
#include <cstring>


rhombus::rhombus() : rhombus(0, 0)
{
}

rhombus::rhombus(size_t a, size_t b)
{
	if (a <= 0 || b <= 0) {
		std::cout << "Incorrect size" << std::endl;
		std::cin >> side_a >> side_b;
	}
	else {
		side_a = a;
		side_b = b;
	}
}

rhombus::rhombus(const rhombus& orig)
{
	side_a = orig.side_a;
	side_b = orig.side_b;
}

int rhombus::F() {  /////
	return 1;      /////
} 

double rhombus::Square()
{
	return side_a * side_b * 0.5;
}

void rhombus::Print()
{
	std::cout << "a = " << side_a << " b = " << side_b << std::endl;
}

std::ostream& operator<<(std::ostream& os, const rhombus& right)
{
	os << "Rhombus with side a = " << right.side_a << " b = " << right.side_b << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, rhombus& right)
{
	is >> right.side_a;
	is >> right.side_b;
	while (right.side_a <= 0 || right.side_b <= 0) {
		std::cout << "Incorrect data. Please repeat enter:" << std::endl;
		is >> right.side_a;
		is >> right.side_b;
	}
	return is;
}

bool operator==(const rhombus& left, const rhombus& right)
{
	if (left.side_a == right.side_a || left.side_b == right.side_b) {
		return true;
	}
	return false;
}

rhombus& rhombus::operator=(const rhombus& right)
{
	side_a = right.side_a;
	side_b = right.side_b;
	return *this;
}


rhombus::~rhombus()
{
}