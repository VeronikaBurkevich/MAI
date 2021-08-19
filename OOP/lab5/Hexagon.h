#ifndef HEXAGON_H
#define HEXAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Hexagon : public Figure
{
public:
	Hexagon();
	Hexagon(size_t side_a);
	Hexagon(const Hexagon& orig);
	Hexagon(std::istream &is);
	double Square() override;
	void Print() override;
	int F() override; 
	friend std::ostream& operator<<(std::ostream& os, const Hexagon& right);
	friend std::istream& operator>>(std::istream& is, Hexagon& right);
	friend bool operator==(const Hexagon& left, const Hexagon& right);
	Hexagon& operator=(const Hexagon& right);
	virtual ~Hexagon();
private:
	size_t side_a;
};
#endif // !HEXAGON_H