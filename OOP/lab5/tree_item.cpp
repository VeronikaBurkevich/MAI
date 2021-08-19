
#include "tree_item.h"
#include <iostream>
#include "Figure.h"

template <class T>
tree_item<T>::tree_item(const std::shared_ptr<T> &data, size_t num)
{
	fig = data;
	bro = nullptr;
	son = nullptr;
	number = num;
}

/*template <class T>
tree_item<T>::tree_item(const std::shared_ptr<T> &orig)
{
	fig = orig->get_figure();
	bro = orig->get_bro();
	son = orig->get_son();
	number = orig->get_number();
}*/

template <class T>
std::shared_ptr<T> tree_item<T>::get_Figure() const
{
	return this->fig;
}

template <class T>
std::shared_ptr<tree_item<T>> tree_item<T>::get_son()
{
	return this->son;
}

template <class T>
std::shared_ptr<tree_item<T>> tree_item<T>::get_bro()
{
	return this->bro;
}

template <class T>
double tree_item<T>::get_data()
{
	return fig->Square();
}

template <class T>
int tree_item<T>::get_number()
{
	return number;
}

template <class T>
void tree_item<T>::set_son(std::shared_ptr<tree_item<T>> Son)
{
	this->son = Son;
}

template <class T>
void tree_item<T>::set_bro(std::shared_ptr<tree_item<T>> Bro)
{
	this->bro = Bro;
}

template <class A>
std::ostream& operator<<(std::ostream& os, const tree_item<A>& obj)
{
	//os << "Figure's type is " << obj.get_figure()->F() << " [rhombus(1), hexagon(2), pentagon(3)]; Square = " << obj.get_figure()->Square() << "; Key:" << obj.number;

	if (obj.get_Figure()->F() == 1)
		os << "Rhombus; Square = " << obj.get_Figure()->Square() << "; Key:" << obj.number;
	if (obj.get_Figure()->F() == 2)
		os << "Pentagon; Square = " << obj.get_Figure()->Square() << "; Key:" << obj.number;
	if (obj.get_Figure()->F() == 3)
		os << "Hexagon; Square = " << obj.get_Figure()->Square() << "; Key:" << obj.number;
	return os;

}

template <class T>
tree_item<T>::~tree_item(){}

template class tree_item<Figure>;
template std::ostream& operator<<(std::ostream& os, const tree_item<Figure>& obj);
