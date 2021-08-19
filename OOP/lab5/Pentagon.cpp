#include <iostream>
#include <cmath>
#include "Pentagon.h"

Pentagon::Pentagon() : Pentagon(0){}

Pentagon::Pentagon(size_t a) : side_a(a){
	//std::cout << "Rectangle created: " << side_a << ", " << side_b << std::endl;
}


Pentagon::Pentagon(const Pentagon& orig){
	//std::cout << "Pentagon copy created." << std::endl;
	side_a = orig.side_a;
}

Pentagon::Pentagon(std::istream & is){
	is >> side_a;
	if (side_a < 0) {
		std::cerr << "Error: sides must be >= 0." << std::endl;
		side_a = 0;
	}
}

int Pentagon::F() {
	return 2;      
}

double Pentagon::Square(){
	return side_a * side_a * 0.25 * sqrt(25 + 10 * sqrt(5));
}

void Pentagon::Print(){
	std::cout << "Pentagon with side a = " << side_a << "and Square = " << Square();
}

Pentagon & Pentagon::operator=(const Pentagon & right){
	if (this == &right)
		return *this;
	//std::cout << "Pentagon copy created." << std::endl;
	side_a = right.side_a;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Pentagon & obj){
	os << "Pentagon with side a = " << obj.side_a;
	return os;
}

std::istream & operator>>(std::istream & is, Pentagon & right){
	is >> right.side_a;
	if (right.side_a < 0) {
		std::cerr << "Error: sides must be >= 0." << std::endl;
		right.side_a = 0;
	}
	return is;
}

bool operator==(const Pentagon & left, const Pentagon &right){
	return (left.side_a == right.side_a);
}

Pentagon::~Pentagon() {}