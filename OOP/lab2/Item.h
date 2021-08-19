#ifndef ITEM_H
#define ITEM_H

#include "Rhombus.h"
class TTreeItem {
public:
	TTreeItem(const Rhombus& rhombus);
	TTreeItem(const TTreeItem &orig);
	friend std::ostream& operator<<(std::ostream &os, const TTreeItem &obj);

	void TTreeItem::SetItem();
	TTreeItem*& GetSon();
	TTreeItem*& GetBrother();
	Rhombus GetRhombus() const;

	virtual ~TTreeItem();
private:
	Rhombus Rhombus;
	TTreeItem *son;
	TTreeItem *brother;
};


#endif /* ITEM_H */