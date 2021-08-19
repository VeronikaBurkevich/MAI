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
	std::shared_ptr<T> get_figure() const;
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