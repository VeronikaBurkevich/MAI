#include <iostream>
#include <cmath>
#include "Hexagon.h"

Hexagon::Hexagon() : Hexagon(0) {}

Hexagon::Hexagon(size_t a) : side_a(a) {
	//std::cout << "Rectangle created: " << side_a << ", " << side_b << std::endl;
}


Hexagon::Hexagon(const Hexagon& orig) {
	//std::cout << "Hexagon copy created." << std::endl;
	side_a = orig.side_a;
}

Hexagon::Hexagon(std::istream & is) {
	is >> side_a;
	if (side_a < 0) {
		std::cerr << "Error: sides must be >= 0." << std::endl;
		side_a = 0;
	}
}

int Hexagon::F() { 
	return 3;      
}

double Hexagon::Square() {
	return side_a * side_a * 1.5 * sqrt(3);
}

void Hexagon::Print() {
	std::cout << "Hexagon with side a = " << side_a << "and Square = " << Square();
}

Hexagon & Hexagon::operator=(const Hexagon & right) {
	if (this == &right)
		return *this;
	//std::cout << "Hexagon copy created." << std::endl;
	side_a = right.side_a;
	return *this;
}

std::ostream & operator<<(std::ostream & os, const Hexagon & obj) {
	os << "Hexagon with side a = " << obj.side_a;
	return os;
}

std::istream & operator>>(std::istream & is, Hexagon & right) {
	is >> right.side_a;
	if (right.side_a < 0) {
		std::cerr << "Error: sides must be >= 0." << std::endl;
		right.side_a = 0;
	}
	return is;
}

bool operator==(const Hexagon & left, const Hexagon &right) {
	return (left.side_a == right.side_a);
}

Hexagon::~Hexagon() {}