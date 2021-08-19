#ifndef ROMBUS_H
#define ROMBUS_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Rombus : public Figure {
public:
    Rombus();
    Rombus(std::istream &is);
    Rombus(size_t d1, size_t d2);
    Rombus(const Rombus& orig);

    virtual double SQUARE() override;
    void Print() override;
    
    virtual ~Rombus();

private:
    size_t side_d1;
    size_t side_d2;
};

#endif /* ROMBUS_H */