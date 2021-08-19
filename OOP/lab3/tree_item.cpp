#include "tree_item.h"
#include "Figure.h"
#include <iostream>
#include <memory>


tree_item::tree_item(const std::shared_ptr<Figure> &data, size_t num)
{
	fig = data;
	bro = nullptr;
	son = nullptr;
	number = num;
}

tree_item::tree_item(const std::shared_ptr<tree_item> &orig)
{
	fig = orig->get_Figure();
	bro = orig->get_bro();
	son = orig->get_son();
	number = orig->get_number();
}

std::shared_ptr<Figure> tree_item::get_Figure() const
{
	return fig;
}

std::shared_ptr<tree_item> tree_item::get_son()
{
	return son;
}

std::shared_ptr<tree_item> tree_item::get_bro()
{
	return bro;
}

size_t tree_item::get_data()
{
	return fig->square();
}

int tree_item::get_number()
{
	return number;
}

void tree_item::set_son(std::shared_ptr<tree_item> Son)
{
	son = Son;
}

void tree_item::set_bro(std::shared_ptr<tree_item> Bro)
{
	bro = Bro;
}

std::ostream& operator<<(std::ostream& os, const tree_item& obj)
{
	os << obj.get_Figure() << " Square = " << obj.get_Figure()->square() << " Number:" << obj.number << std::endl;
	return os;
}

tree_item::~tree_item(){}
