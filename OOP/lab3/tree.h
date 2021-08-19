#pragma once
#include <memory>
#include "tree_item.h"
#include "Figure.h"
class tree
{
public:
	tree();
	std::shared_ptr<tree_item> insert(std::shared_ptr<tree_item> node, size_t key);
	void remove(size_t key);
	void remove(std::shared_ptr<tree_item> tree, size_t key);
	bool empty();
	std::shared_ptr<tree_item> find(std::shared_ptr<tree_item> tree, size_t key);
	void print();
	void print(std::shared_ptr<tree_item> tree, size_t depth);
	friend std::ostream& operator<<(std::ostream& os, tree& obj);
	~tree();
private:
	std::shared_ptr<tree_item> root;
};

