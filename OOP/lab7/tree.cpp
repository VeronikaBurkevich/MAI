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
std::shared_ptr<tree_item<T>> tree<T>::insert(std::shared_ptr<T> figure, size_t parentKey, size_t key)
{
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
std::shared_ptr<tree_item<T>> tree<T>::getRoot()
{
	return root;
}

template <class T>
void tree<T>::remove(size_t key)
{
	if (root->getKey() == key) {
		root = nullptr;
	}
	else {
		remove(root, key);
	}
}


template <class T>
void tree<T>::remove(std::shared_ptr<tree_item<T>> node, size_t key)
{
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
bool tree<T>::empty()
{
	return this->root == nullptr;
}

template <class T>
std::shared_ptr<tree_item<T>> tree<T>::find(std::shared_ptr<tree_item<T>> node, size_t key)
{
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
		if (tr != nullptr) {
			return tr;
		}
	}
	return nullptr;
}

template <class T>
void tree<T>::print()
{
	this->print(this->root, 0);
}

template <class T>
void tree<T>::print(std::shared_ptr<tree_item<T>> tree, size_t depth)
{
	if (tree)
		for (size_t i = 0; i < depth; i++)
			std::cout << "\t";
	tree->print();
	if (tree->getSon())
		print(tree->getSon(), depth + 1);
	if (tree->getSibling())
		print(tree->getSibling(), depth);
}

template<class T>
void tree<T>::quickSort(size_t arr[], size_t low, size_t high)
{
	if (low < high)
	{

		size_t pi = partition(arr, low, high);
		if (pi > 0)
			quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

template<class T>
void tree<T>::sort()
{

	size_t size = 0;
	for (auto it : *this) {
		size++;
	}
	//std::shared_ptr<tree_item<T>> * arr = new std::shared_ptr<tree_item<T>>[size];
	size_t arr[100];
	auto it = begin();
	for (size_t i = 0; i < size; ++i) {
		arr[i] = it->getKey();
		it++;
	}
	it = begin();
	quickSort(arr, 0, size - 1);
	for (size_t i = 0; i < size; ++i) {
		it->setKey(arr[i]);
		it++;
	}
}

size_t FutureQuickSort(size_t *arr, size_t left, size_t right)
{
	size_t i = left, j = right;

	size_t tmp;

	size_t pivot = arr[(left + right) / 2];

	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	if (i < right) {
		std::packaged_task<size_t(size_t *, size_t, size_t)> task(FutureQuickSort);
		auto result = task.get_future();

		std::thread task_td(std::move(task), std::ref(arr), i, right);
		task_td.join();
		result.get();
	}
	if (left < j) {
		std::packaged_task<size_t(size_t *, size_t, size_t)> task(FutureQuickSort);
		auto result = task.get_future();

		std::thread task_td(std::move(task), std::ref(arr), left, j);
		task_td.join();
		result.get();
	}
	return 0;
}


template<class T>
void tree<T>::sortParallel()
{
	size_t size = 0;
	for (auto it : *this) {
		size++;
	}
	//std::shared_ptr<tree_item<T>> * arr = new std::shared_ptr<tree_item<T>>[size];
	size_t arr[100];
	auto it = begin();
	for (size_t i = 0; i < size; ++i) {
		arr[i] = it->getKey();
		it++;
	}
	FutureQuickSort(arr, 0, size - 1);
	it = begin();
	for (size_t i = 0; i < size; ++i) {
		it->setKey(arr[i]);
		it++;
	}

}


template <class T>
tree<T>::~tree()
{
}

void swap(size_t* a, size_t* b)
{
	size_t t = *a;
	*a = *b;
	*b = t;
}

template<class T>
size_t tree<T>::partition(size_t arr[], size_t low, size_t high)
{
	int pivot = arr[high];    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; high != 0 && j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++;    // increment index of smaller element
			//swap(&arr[i], &arr[j]);
			size_t tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}
	//swap(&arr[i + 1], &arr[high]);
	size_t tmp = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = tmp;
	std::cout << i + 1 << std::endl;
	return (i + 1);
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
