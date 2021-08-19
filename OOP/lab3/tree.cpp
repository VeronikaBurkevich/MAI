#include <iostream>
#include <cmath>
#include "tree.h"



tree::tree()
{
	root = nullptr;
}

std::shared_ptr<tree_item> tree::insert(std::shared_ptr<tree_item> node, size_t key)
{
	if (this->empty()) {
		this->root = node;
		std::cout << "Tree was empty. Item was setted as root" << std::endl;
		return this->root;
	}
	std::shared_ptr<tree_item> parent = find(root, key);
	if (parent == nullptr) {
		std::cout << "Parent number incorrect." << std::endl;
		return nullptr;
	}
	if (parent->get_son() != nullptr) {
		std::shared_ptr<tree_item> tmp = parent->get_son();
		while (tmp->get_bro()) {
			tmp = tmp->get_bro();
		}
		tmp->set_bro(node);
		return tmp->get_bro();
	}
	else {
		parent->set_son(node);
		return parent->get_son();
	}
}

void tree::remove(size_t key)
{
	if (root->get_number() == key) {
		root = nullptr;
	}
	else {
		remove(root, key);
	}
}

void tree::remove(std::shared_ptr<tree_item> tree, size_t key)
{
	if (tree->get_son()) {
		if (tree->get_son()->get_number() == key) {
			std::shared_ptr<tree_item> tr = tree->get_son();
			tree->set_son(tree->get_son()->get_bro());
			tr->set_bro(nullptr);
			return;
		}
		else {
			remove(tree->get_son(), key);
		}
	}
	if (tree->get_bro()) {
		if (tree->get_bro()->get_number() == key) {
			std::shared_ptr<tree_item> tr = tree->get_bro();
			tree->set_bro(tree->get_bro()->get_bro());
			tr->set_bro(nullptr);
			return;
		}
		else {
			remove(tree->get_bro(), key);
		}
	}
	return;
}

bool tree::empty()
{
	return this->root == nullptr;
}

std::shared_ptr<tree_item> tree::find(std::shared_ptr<tree_item> tree, size_t key)
{
	if (tree->get_number() == key) {
		return tree;
	}
	std::shared_ptr<tree_item> tr = nullptr;
	if (tree->get_son()) {
		tr = find(tree->get_son(), key);
		if (tr != nullptr) {
			return tr;
		}
	}
	if (tree->get_bro()) {
		tr = find(tree->get_bro(), key);
		if (tr != nullptr) {
			return tr;
		}
	}
	return nullptr;
}

void tree::print()
{
	print(root, 0);
}

void tree::print(std::shared_ptr<tree_item> tree, size_t depth)
{
	for (int i = 0; i < depth; ++i)
		std::cout << "--";
	std::cout << *tree << std::endl;
	if (tree->get_son()) 
		print(tree->get_son(), depth + 1);
	if (tree->get_bro()) 
		print(tree->get_bro(), depth);
}

std::ostream& operator<<(std::ostream& os, tree& obj)
{
	obj.print();
	return os;
}

tree::~tree()
{
}
