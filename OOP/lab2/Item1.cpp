#include <iostream>
#include <cstdlib>

#include "Item1.h"
#include "Rhombus.h"

TTreeItem::TTreeItem(const Rhombus &rhombus) {
	this->Rhombus = rhombus;
	this->son = nullptr;
	this->brother = nullptr;
	std::cout << "Tree item: created" << std::endl;
}

TTreeItem