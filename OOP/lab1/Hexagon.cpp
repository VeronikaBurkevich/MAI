#include "Hexagon.h"
#include <iostream>
#include <cmath>
#define PI 3.141592
 
Hexagon::Hexagon() : Hexagon(0) {
}
 
Hexagon::Hexagon(size_t r) : side(r) {
	std::cout << "Hexagon created: " << side << std::endl;
}
 
Hexagon::Hexagon(std::istream &is) {
	is >> side;
}
 
Hexagon::Hexagon(const Hexagon& orig) {
	std::cout << "Hexagon copy created" << std::endl;
	side = orig.side;
}
 
double Hexagon::SQUARE() {
	return (double)((2 * side * side) / (double)tan(30 * (PI / 180)));
}
 
void Hexagon::Print() {
	std::cout << "r =" << side <<  std::endl;
 
}
 
Hexagon::~Hexagon() {
	std::cout << "Hexagon deleted" << std::endl;
}
