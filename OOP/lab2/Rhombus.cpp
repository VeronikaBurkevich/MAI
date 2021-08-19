#include "Rhombus.h"
#include <iostream>
#include <cmath>

Rhombus::Rhombus() : Rhombus(0, 0) { //ромб
}

Rhombus::Rhombus(size_t d1, size_t d2) : side_d1(d1), side_d2(d2) {
	std::cout << "Rhombus created: " << side_d1 << side_d2 << std::endl;
}

Rhombus::Rhombus(std::istream &is) {
	int ptr1, ptr2;
	std::cout << "Insert the longest and the shortest diagonals:" << std::endl;
	is >> ptr1;
	is >> ptr2;
	while (ptr1 < 0 || ptr2 < 0){
		std::cout << "Please, insert correct data" << std::endl;
		is >> ptr1;
		is >> ptr2;
	}
	side_d1 = ptr1;
	side_d2 = ptr2;
}

Rhombus::Rhombus(const Rhombus& orig) {
	std::cout << "Rhombus copy created" << std::endl;
	side_d1 = orig.side_d1;
	side_d2 = orig.side_d2;
}

double Rhombus::SQUARE() {
	return side_d1 * side_d2 * 0.5;
}


void Rhombus::Print() {
	std::cout << "d1 = " << side_d1 << ", d2 = " << side_d2 << std::endl;

}


Rhombus::~Rhombus() {
	std::cout << "Rhombus deleted" << std::endl;
}
