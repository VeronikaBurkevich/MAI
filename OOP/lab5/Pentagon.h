#ifndef PENTAGON_H
#define PENTAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Pentagon : public Figure
{
public:
	Pentagon();
	Pentagon(size_t side_a);
	Pentagon(const Pentagon& orig);
	Pentagon(std::istream &is);
	double Square() override;
	void Print() override;
	int F() override; 
	friend std::ostream& operator<<(std::ostream& os, const Pentagon& right);
	friend std::istream& operator>>(std::istream& is, Pentagon& right);
	friend bool operator==(const Pentagon& left, const Pentagon& right);
	Pentagon& operator=(const Pentagon& right);
	virtual ~Pentagon();
private:
	size_t side_a;
};
#endif // !PENTAGON_H