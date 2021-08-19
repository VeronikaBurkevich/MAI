#include <iostream>
#include <cmath>
#include "Hexagon.h"


hexagon::hexagon() : hexagon(0) {}

hexagon::hexagon(size_t a) {
	if (a <= 0) {
		std::cout << "Incorrect size" << std::endl;
	}
	side_a = a;
}

hexagon::hexagon(const hexagon& orig) {
	side_a = orig.side_a;
}

int hexagon::F() {  /////
	return 2;      /////
}

double hexagon::Square() {
	return side_a * side_a * 1.5 * sqrt(3);
}

void hexagon::Print() {
	std::cout << "a = " << side_a << std::endl;
}

std::ostream& operator<<(std::ostream& os, const hexagon& right) {
	os << "hexagon with side a = " << right.side_a << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, hexagon& right) {
	is >> right.side_a;
	while (right.side_a <= 0) {
		std::cout << "Incorrect data. Please repeat enter:" << std::endl;
		is >> right.side_a;
	}
	return is;
}

bool operator==(const hexagon& left, const hexagon& right) {
	if (left.side_a == right.side_a) {
		return true;
	}
	return false;
}

hexagon& hexagon::operator=(const hexagon& right) {
	side_a = right.side_a;
	return *this;
}

hexagon::~hexagon() {}