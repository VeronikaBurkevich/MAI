#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include "Pentagon.h"
#include "Rhombus.h"
#include "Hexagon.h"
#include <memory>

template<class T> class tree_item
{
public:
	tree_item(const std::shared_ptr<T> &data, size_t num);
	//tree_item(const std::shared_ptr<T> &orig);
	std::shared_ptr<T> get_Figure() const;
	std::shared_ptr<tree_item<T>> get_son();
	std::shared_ptr<tree_item<T>> get_bro();
	double get_data();
	int get_number();
	void set_son(std::shared_ptr<tree_item<T>> son);
	void set_bro(std::shared_ptr<tree_item<T>> bro);
	template <class A> friend std::ostream& operator<<(std::ostream& os, const tree_item<A>& obj);

	~tree_item();
private:
	std::shared_ptr<T> fig;
	std::shared_ptr<tree_item<T>> son;
	std::shared_ptr<tree_item<T>> bro;
	int number;
};
#endif // !TREE_ITEM_H


/*#ifndef tree_ITEM_H
#define tree_ITEM_H
#include "Pentagon.h"
#include "Rhombus.h"
#include "Hexagon.h"
#include <memory>

template <class T> class tree_item
{
public:
	tree_item(const std::shared_ptr<T>& figure, size_t key);
	tree_item(const tree_item& orig);


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
};
#endif // !tree_ITEM_H
*/