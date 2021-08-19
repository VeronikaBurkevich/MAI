//#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "Rhombus.h"

rhombus::rhombus() : rhombus(0, 0) {}

rhombus::rhombus(size_t d1, size_t d2) {
	if (d1 <= 0 || d2 <= 0) {
		std::cout << "Incorrect size" << std::endl;
		std::cin >> side_d1 >> side_d2;
	}
	else {
		side_d1 = d1;
		side_d2 = d2;
	}
}

rhombus::rhombus(const rhombus& orig) {
	side_d1 = orig.side_d1;
	side_d2 = orig.side_d2;
}

double rhombus::square() {
	return side_d1 * side_d2 * 0.5;
}

void rhombus::print() {
	std::cout << "d1 = " << side_d1 << " d2 = " << side_d2 << std::endl;
}

char rhombus::figur()
{
	return rhombus;
}

std::ostream& operator<<(std::ostream& os, const rhombus& right) {
	os << "Rhombus with diagonales: d1= " << right.side_d1 << " d2 = " << right.side_d2 << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, rhombus& right) {
	is >> right.side_d1;
	is >> right.side_d2;
	while (right.side_d1 <= 0 || right.side_d2 <= 0) {
		std::cout << "Incorrect data. Please repeat enter:" << std::endl;
		is >> right.side_d1;
		is >> right.side_d2;
	}
	return is;
}

bool operator==(const rhombus& left, const rhombus& right) {
	if (left.side_d1 == right.side_d1 || left.side_d2 == right.side_d2) {
		return true;
	}
	return false;
}

rhombus& rhombus::operator=(const rhombus& right) {
	side_d1 = right.side_d1;
	side_d2 = right.side_d2;
	return *this;
}


rhombus::~rhombus() {}