#ifndef Rhombus_H
#define Rhombus_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Rhombus : public Figure {
public:

	/*typedef struct{
		size_t d1;
		size_t d2;
	}rhombus;*/

	Rhombus();
	Rhombus(std::istream &is);
	Rhombus(size_t d1, size_t d2);
	Rhombus(const Rhombus& orig);
	virtual double SQUARE() override;
	void Print() override;

	virtual ~Rhombus();

private:
	//size_t side_d;
	size_t side_d1;
	size_t side_d2;
};

#endif /* Rhombus_H */