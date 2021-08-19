#ifndef TREE_ITEM_H
#define TREE_ITEM_H
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "AllocationBlock.h"
#include <memory>

template <class T> class tree_item
{
public:
	tree_item(const std::shared_ptr<T>& figure, size_t key);
	tree_item(const tree_item& orig);

	void * operator new (size_t size);
	void operator delete(void *p);

	std::shared_ptr<tree_item<T>> getParent();
	std::shared_ptr<tree_item<T>> getSon();
	std::shared_ptr<tree_item<T>> getSibling();
	void setParent(std::shared_ptr<tree_item<T>> parent);
	void setSon(std::shared_ptr<tree_item<T>> son);
	void setSibling(std::shared_ptr<tree_item<T>> sibling);
	void setKey(size_t key);
	size_t getKey() const;

	void print();

	std::shared_ptr<T> getFigure() const;
	template <class A> friend std::ostream& operator<<(std::ostream& os, const tree_item<A>& obj);

	std::shared_ptr<tree_item<T>> getNext();

	~tree_item();
private:
	std::shared_ptr<tree_item<T>> son;
	std::shared_ptr<tree_item<T>> parent;
	std::shared_ptr<tree_item<T>> sibling;
	std::shared_ptr<T> figure;
	size_t key;

	static TAllocationBlock tree_itemAllocator;
};
#endif // !TREE_ITEM_H
