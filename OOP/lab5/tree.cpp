#include "tree.h"

template <class T>
tree<T>::tree()
{
	this->root = nullptr;
}

template<class T>
TIterator<tree_item<T>, T> tree<T>::begin()
{
	return TIterator<tree_item<T>, T>(root);
}

template<class T>
TIterator<tree_item<T>, T> tree<T>::end()
{
	return TIterator<tree_item<T>, T>(nullptr);
}

template <class T>
std::shared_ptr<tree_item<T>> tree<T>::insert(std::shared_ptr<tree_item<T>> node, size_t key)
{	
	if (this->empty()) {
		this->root = node;
		std::cout << "Tree was empty. Item was setted as root" << std::endl;
		return this->root;
	}
	std::shared_ptr<tree_item<T>> parent = find(root, key);
	if (parent == nullptr) {
		std::cout << "Parent number incorrect." << std::endl;
		return nullptr;
	}
	if (parent->get_son() != nullptr) {
		std::shared_ptr<tree_item<T>> tmp = parent->get_son();
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


template<class T>
std::shared_ptr<tree_item<T>> tree<T>::getRoot()
{
	return root;
}

template <class T>
void tree<T>::remove(size_t key)
{
	if (root->get_number() == key) {
		root = nullptr;
	}
	else {
		remove(root, key);
	}
}

template <class T>
void tree<T>::remove(std::shared_ptr<tree_item<T>> tree, size_t key)
{
	if (tree->get_son()) {
		if (tree->get_son()->get_number() == key) {
			std::shared_ptr<tree_item<T>> tr = tree->get_son();
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
			std::shared_ptr<tree_item<T>> tr = tree->get_bro();
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

template <class T>
bool tree<T>::empty()
{
	return this->root == nullptr;
}

template <class T>
std::shared_ptr<tree_item<T>> tree<T>::find(std::shared_ptr<tree_item<T>> tree, size_t key)
{
	if (tree->get_number() == key) {
		return tree;
	}
	std::shared_ptr<tree_item<T>> tr = nullptr;
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

template <class T>
void tree<T>::Print()
{
	Print(root, 0);
}

template <class T>
void tree<T>::Print(std::shared_ptr<tree_item<T>> tree, size_t depth)
{
	for (int i = 0; i < depth; ++i) {
		std::cout << "--";
	}
	std::cout << *tree << std::endl;
	if (tree->get_son()) Print(tree->get_son(), depth + 1);
	if (tree->get_bro()) Print(tree->get_bro(), depth);
}

template <class T>
tree<T>::~tree()
{
}

template <class T>
std::ostream & operator<<(std::ostream & os, tree<T> & obj)
{
	obj.print();
	return os;
}

#include "Figure.h"
template class tree<Figure>;
template std::ostream& operator<<(std::ostream& os, const tree<Figure>& tree);