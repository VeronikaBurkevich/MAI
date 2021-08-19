#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include"Figure.h"
#include<memory>

class tree_item
{
public:
	tree_item(const std::shared_ptr<Figure> &data, size_t num);
	tree_item(const std::shared_ptr<tree_item> &orig);
	std::shared_ptr<Figure> get_Figure() const;
	std::shared_ptr<tree_item> get_son();
	std::shared_ptr<tree_item> get_bro();
	size_t get_data();
	int get_number();
	void set_son(std::shared_ptr<tree_item> son);
	void set_bro(std::shared_ptr<tree_item> bro);
	friend std::ostream& operator<<(std::ostream& os, const tree_item& obj);

	~tree_item();
private:
	std::shared_ptr<Figure> fig;
	std::shared_ptr<tree_item> son;
	std::shared_ptr<tree_item> bro;
	int number;
};

#endif 