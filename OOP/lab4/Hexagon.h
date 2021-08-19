#ifndef HEXAGON_H
#define HEXAGON_H

#include <cstdlib>
#include <iostream>
#include "Figure.h"
class hexagon : public figure
{
public:
	hexagon();
	hexagon(std::istream &is);
	hexagon(size_t a);
	hexagon(const hexagon& orig);

	int F() override; //////////////////
	double Square() override;
	void Print() override;
	friend std::ostream& operator<<(std::ostream& os, const hexagon& right);
	friend std::istream& operator>>(std::istream& is, hexagon& right);
	friend bool operator==(const hexagon& left, const hexagon& right);
	hexagon& operator=(const hexagon& right);
	~hexagon();
private:
	size_t side_a;
};
#endif