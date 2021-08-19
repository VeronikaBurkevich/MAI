#ifndef FIGURE_H
#define FIGURE_H

class figure {
public:
	virtual double Square() = 0;
	virtual void Print() = 0;
	virtual ~figure() {};
	virtual int F() = 0; //////////
};
#endif