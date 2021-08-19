#ifndef PENTAGON_H
#define PENTAGON_H

#include <cstdlib>
#include <iostream>
#include "Figure.h"
class pentagon :
	public figure
{
public:
	pentagon();
	pentagon(std::istream &is);
	pentagon(size_t a);
	pentagon(const pentagon& orig);

	int F() override; //////////////////
	double Square() override;
	void Print() override;
	friend std::ostream& operator<<(std::ostream& os, const pentagon& right);
	friend std::istream& operator>>(std::istream& is, pentagon& right);
	friend bool operator==(const pentagon& left, const pentagon& right);
	pentagon& operator=(const pentagon& right);
	~pentagon();
private:
	size_t side_a;
};
#endif // !PENTAGON_H