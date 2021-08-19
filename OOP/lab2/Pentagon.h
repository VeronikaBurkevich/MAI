#ifndef Pentagon_H
#define Pentagon_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Pentagon : public Figure {
public:
	Pentagon();
	Pentagon(std::istream &is);
	Pentagon(size_t side);
	size_t GetSide();
	Pentagon(const Pentagon& orig);

	virtual double SQUARE() override;
	void Print() override;

	virtual ~Pentagon();

private:
	size_t side;
};

#endif /* Pentagon_H */