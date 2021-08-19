#include "tree.h"

template <class T>
tree<T>::tree(){
	this->root = nullptr;
}

template<class T>
TIterator<tree_item<T>, T> tree<T>::begin(){
	return TIterator<tree_item<T>, T>(root);
}

template<class T>
TIterator<tree_item<T>, T> tree<T>::end(){
	return TIterator<tree_item<T>, T>(nullptr);
}

template <class T>
std::shared_ptr<tree_item<T>> tree<T>::insert(std::shared_ptr<T> figure, size_t parentKey, size_t key){
	std::shared_ptr<tree_item<T>> node = std::make_shared<tree_item<T>>(figure, key);
	if (empty()) {
		root = node;
		root->setParent(nullptr);
		std::cout << "tree was empty. Item was setted as root." << std::endl;
		return root;
	}
	std::shared_ptr<tree_item<T>> parent = find(this->root, parentKey);
	if (!parent) {
		std::cout << "Parent with this key not found. Automatic insertion to the nearest place." << std::endl;
		if (root->getSon()) {
			std::shared_ptr<tree_item<T>> tmp = root->getSon();
			while (tmp->getSibling())
				tmp = tmp->getSibling();
			tmp->setSibling(node);
			node->setParent(root);
			return tmp->getSibling();
		}
		else {
			root->setSon(node);
			node->setParent(root);
			return root->getSon();
		}
	}
	if (parent->getSon()) {
		std::shared_ptr<tree_item<T>> tmp = parent->getSon();
		while (tmp->getSibling())
			tmp = tmp->getSibling();
		tmp->setSibling(node);
		return tmp->getSibling();
	}
	else {
		parent->setSon(node);
		node->setParent(parent);
		return parent->getSon();
	}
}

template<class T>
std::shared_ptr<tree_item<T>> tree<T>::getRoot(){
	return root;
}

template <class T>
void tree<T>::remove(size_t key){
	if (root->getKey() == key) {
		root = nullptr;
	}
	else {
		remove(root, key);
	}
}

template <class T>
void tree<T>::remove(std::shared_ptr<tree_item<T>> node, size_t key){
	if (node->getSon()) {
		if (node->getSon()->getKey() == key) {
			std::shared_ptr<tree_item<T>> tr = node->getSon();
			node->setSon(node->getSon()->getSibling());
			tr->setSibling(nullptr);
			return;
		}
		else {
			remove(node->getSon(), key);
		}
	}
	if (node->getSibling()) {
		if (node->getSibling()->getKey() == key) {
			std::shared_ptr<tree_item<T>> tr = node->getSibling();
			node->setSibling(node->getSibling()->getSibling());
			tr->setSibling(nullptr);
			return;
		}
		else {
			remove(node->getSibling(), key);
		}
	}
}

template <class T>
bool tree<T>::empty(){
	return this->root == nullptr;
}

template <class T>
std::shared_ptr<tree_item<T>> tree<T>::find(std::shared_ptr<tree_item<T>> node, size_t key){
	std::shared_ptr<tree_item<T>> tr = nullptr;
	if (node->getKey() == key)
		return node;
	if (node->getSon()) {
		tr = find(node->getSon(), key);
		if (tr != nullptr) {
			return tr;
		}
	}
	if (node->getSibling()) {
		tr = find(node->getSibling(), key);
		if (tr != nullptr)
			return tr;
	}
	return nullptr;
}

template <class T>
void tree<T>::print(){
	this->print(this->root, 0);
}

template <class T>
void tree<T>::print(std::shared_ptr<tree_item<T>> tree, size_t depth){
	if (tree)
		for (size_t i = 0; i < depth; i++)
			std::cout << "\t";
	tree->print();
	if (tree->getSon())
		print(tree->getSon(), depth + 1);
	if (tree->getSibling())
		print(tree->getSibling(), depth);
}

template <class T>
tree<T>::~tree(){}

template <class T>
std::ostream & operator<<(std::ostream & os, tree<T> & obj){
	obj.print();
	return os;
}

#include "Figure.h"
template class tree<Figure>;
template std::ostream& operator<<(std::ostream& os, const tree<Figure>& tree);