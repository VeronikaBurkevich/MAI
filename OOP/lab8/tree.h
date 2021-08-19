#ifndef TREE_H
#define TREE_H
#include "tree_item.h"
#include "iterator.h"
#include <memory>
#include <mutex>
#include <future>

template <class T> class tree
{
public:
	tree();

	TIterator<tree_item<T>, T> begin();
	TIterator<tree_item<T>, T> end();
	std::shared_ptr<tree_item<T>> insert(std::shared_ptr<T> figure, size_t parentKey, size_t key);
	std::shared_ptr<tree_item<T>> getRoot();
	void remove(size_t key);
	void remove(std::shared_ptr<tree_item<T>> tree, size_t key);
	bool empty();
	std::shared_ptr<tree_item<T>> find(std::shared_ptr<tree_item<T>> tree, size_t key);
	void print();
	void print(std::shared_ptr<tree_item<T>> tree, size_t depth);
	template <class A> friend std::ostream& operator<<(std::ostream& os, const tree<A> & obj);
	void sort();
	void sortParallel();

	~tree();
private:
	std::future<void> sortInBackground();
	size_t partition(size_t arr[], size_t low, size_t high);
	void quickSort(size_t arr[], size_t low, size_t high);
	std::shared_ptr<tree_item<T>> root;
};


#endif // !TREE_H