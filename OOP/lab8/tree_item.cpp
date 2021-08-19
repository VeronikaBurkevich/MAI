#include "tree_item.h"
#include <iostream>


template<class T>
tree_item<T>::tree_item(const std::shared_ptr<T>& figure, size_t key)
{
	this->figure = figure;
	this->key = key;
	this->sibling = nullptr;
	this->son = nullptr;
	this->parent = nullptr;
}

template <class T> TAllocationBlock
tree_item<T>::tree_itemAllocator(sizeof(tree_item<T>), 100);

template <class T>
tree_item<T>::tree_item(const tree_item<T>& orig)
{
	this->figure = orig.figure;
	this->son = orig.son;
	this->sibling = orig.sibling;
	this->parent = orig.parent;
}

template<class T>
void * tree_item<T>::operator new(size_t size)
{
	return tree_itemAllocator.allocate();
}

template<class T>
void tree_item<T>::operator delete(void * p)
{
	tree_itemAllocator.deallocate(p);
}

template<class T>
std::shared_ptr<tree_item<T>> tree_item<T>::getParent()
{
	return this->parent;
}

template <class T>
std::shared_ptr<tree_item<T>> tree_item<T>::getSon()
{
	return this->son;
}

template <class T>
std::shared_ptr<tree_item<T>> tree_item<T>::getSibling()
{
	return this->sibling;
}

template<class T>
void tree_item<T>::setParent(std::shared_ptr<tree_item<T>> parent)
{
	this->parent = parent;
}

template <class T>
void tree_item<T>::setSon(std::shared_ptr<tree_item<T>> son)
{
	this->son = son;
}

template <class T>
void tree_item<T>::setSibling(std::shared_ptr<tree_item<T>> sibling)
{
	this->sibling = sibling;
	sibling->parent = this->parent;
}

template <class T>
std::shared_ptr<T> tree_item<T>::getFigure() const
{
	return this->figure;
}

template <class T>
void tree_item<T>::setKey(size_t key)
{
	this->key = key;
}

template<class T>
void tree_item<T>::setFigure(std::shared_ptr<T> figure)
{
	this->figure = figure;
}

template <class T>
size_t tree_item<T>::getKey() const
{
	return key;
}

template<class T>
void tree_item<T>::print()
{
	figure->Print();
	std::cout << ", Square = " << figure->Square() << ", Key = " << key << std::endl;
}

template<class T>
std::shared_ptr<tree_item<T>> tree_item<T>::getNext()
{
	std::shared_ptr<tree_item<T>> tmp;
	if (this->getSon()) {
		return getSon();
	}
	else if (this->getSibling()) {
		return getSibling();
	}
	else if (this->getParent()) {
		tmp = this->getParent();

		while (tmp && !(tmp->getSibling())) {
			tmp = tmp->getParent();
		}
		if (!tmp)
			return nullptr;
		tmp = tmp->getSibling();
		return tmp;
	}
	return nullptr;
}

template <class T>
tree_item<T>::~tree_item()
{
	//std::cout << "tree_item deleted" << std::endl;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const tree_item<T>& obj)
{
	//os << "[" << *obj.getFigure() << "]" << ":"<< obj.getKey() << std::endl;
	return os;
}

#include "tree_item.h"
template class tree_item<Figure>;
template std::ostream& operator<<(std::ostream& os, const tree_item<Figure>& obj);