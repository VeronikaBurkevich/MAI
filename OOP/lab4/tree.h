#ifndef TREE_H
#define TREE_H

#include "tree_item.h"
#include "figure.h"
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include <memory>
#include <iostream>


template <class T> class tree
{
public:
	tree();
	std::shared_ptr<tree_item<T>> insert(std::shared_ptr<tree_item<T>> node, size_t key);
	void remove(size_t key);
	void remove(std::shared_ptr<tree_item<T>> tree, size_t key);
	bool empty();
	std::shared_ptr<tree_item<T>> find(std::shared_ptr<tree_item<T>> tree, size_t key);
	void print();
	void print(std::shared_ptr<tree_item<T>> tree, size_t depth);
	template <class A> friend std::ostream& operator<<(std::ostream& os, tree<A>& obj);
	~tree();
private:
	std::shared_ptr<tree_item<T>> root;
};

#endif // !TREE_H