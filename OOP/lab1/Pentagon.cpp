#include "Pentagon.h"
#include <iostream>
#include <cmath>
#define PI 3.141592
 
Pentagon::Pentagon() : Pentagon(0) {  //    пятиугольник
}
 
Pentagon::Pentagon(size_t r) : side(r) {
	std::cout << "Pentagon created: " << side << std::endl;
}
 
Pentagon::Pentagon(std::istream &is) {
	is >> side;
}
 
Pentagon::Pentagon(const Pentagon& orig) {
	std::cout << "Pentagon copy created" << std::endl;
	side = orig.side;
}
 
double Pentagon::SQUARE() {
	return (double)((2 * side * side) / (double)tan(36 * (PI / 180)));
}
 
void Pentagon::Print() {
	std::cout << "r =" << side <<  std::endl;
 
}
 
Pentagon::~Pentagon() {
	std::cout << "Pentagon deleted" << std::endl;
}
