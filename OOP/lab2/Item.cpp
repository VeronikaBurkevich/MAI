#include <iostream>
#include <cstdlib>

#include "Item1.h"
#include "Rhombus.h"

TTreeItem::TTreeItem(const Rhombus& Rhombus) {
	this->Rhombus = Rhombus;
	this->son = nullptr;
	this->brother = nullptr;
	std::cout << "Tree item: created" << std::endl;
}

TTreeItem::TTreeItem(const TTreeItem &orig) {
	this->Rhombus = orig.Rhombus;
	this->son = orig.son;
	this->brother = orig.brother;
	std::cout << "Tree item: copied" << std::endl;
}

void TTreeItem::SetItem(Rhombus &Rhombus) {
	this->Rhombus = Rhombus;
	this->son = nullptr;
	this->brother = nullptr;
	return;
}

Rhombus TTreeItem::GetRhombus() const {
	return this->Rhombus;
}

TTreeItem*& TTreeItem::GetSon() {
	return this->son;
}

TTreeItem*& TTreeItem::GetBrother() {
	return this->brother;
}

TTreeItem::~TTreeItem() {
	std::cout << "Tree item: deleted" << std::endl;
	son = nullptr;
	brother = nullptr;
}

std::ostream& operator<<(std::ostream &os, const TTreeItem &obj) {
	os << "[" << obj.GetRhombus().SQUARE() << "]";
	return os;
}