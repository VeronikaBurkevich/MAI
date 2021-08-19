#ifndef RHOMBUS_H
#define RHOMBUS_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class rhombus : public Figure{
public:
	rhombus();
	rhombus(std::istream &is);
	rhombus(size_t d1, size_t d2);
	rhombus(const rhombus& orig);

	double square();
	void print();
	char figur();
	//size_t get_value_d1();
	//size_t get_value_d2();
	friend std::ostream& operator<<(std::ostream& os, const rhombus& right);
	friend std::istream& operator>>(std::istream& is, rhombus& right);
	friend bool operator==(const rhombus& left, const rhombus& right);
	rhombus& operator=(const rhombus& right);
	~rhombus();
private:
	size_t side_d1;
	size_t side_d2;
};
#endif 