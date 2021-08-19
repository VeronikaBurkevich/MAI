#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <cstdlib>
#include <iostream>
#include "Figure.h"

class rhombus : public figure
{
public:
	rhombus();
	rhombus(std::istream &is);
	rhombus(size_t a, size_t b);
	rhombus(const rhombus& orig);

	int F() override; //////////////////
	double Square() override;
	void Print() override;
	//size_t get_value_h();
	//size_t get_value_a();
	friend std::ostream& operator<<(std::ostream& os, const rhombus& right);
	friend std::istream& operator>>(std::istream& is, rhombus& right);
	friend bool operator==(const rhombus& left, const rhombus& right);
	rhombus& operator=(const rhombus& right);
	~rhombus();
private:
	size_t side_a;
	size_t side_b;
};

#endif