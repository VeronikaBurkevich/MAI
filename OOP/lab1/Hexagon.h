#ifndef Hexagon_H
#define Hexagon_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Hexagon : public Figure {
public:
    Hexagon();
    Hexagon(std::istream &is);
    Hexagon(size_t side);
    Hexagon(const Hexagon& orig);

    virtual double SQUARE() override;
    void Print() override;
    
    virtual ~Hexagon();

private:
    size_t side;
};

#endif /* Hexagon_H */