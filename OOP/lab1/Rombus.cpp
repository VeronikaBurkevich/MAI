#include "Rombus.h"
#include <iostream>
#include <cmath>
 
Rombus::Rombus() : Rombus(0, 0) {
}
 
Rombus::Rombus(size_t d1, size_t d2) : side_d1(d1), side_d2(d2) {
	std::cout << "Rombus created: " << side_d1 << ", " << side_d2 << ", " << std::endl;
}
 
Rombus::Rombus(std::istream &is) {
	is >> side_d1;
	is >> side_d2;
}
 
Rombus::Rombus(const Rombus& orig) {
	std::cout << "Rombus copy created" << std::endl;
	side_d1 = orig.side_d1;
	side_d2 = orig.side_d2;
}
 
double Rombus::SQUARE() {
	return side_d1 * side_d2 * 0.5;
}
 
 
void Rombus::Print() {
	std::cout << "d1 = " << side_d1 << ", d2 = " << side_d2 << std::endl;
 
}
 
Rombus::~Rombus() {
	std::cout << "Rombus deleted" << std::endl;
}
