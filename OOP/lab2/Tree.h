#ifndef TREE_H
#define TREE_H

#include "Rhombus.h"
#include "Item1.h"
#include <stdbool.h>

class TTree{
public:
	TTree();
	TTree(const TTree &orig);

	int GetNodesOnLevel(int level) {
		return GetNodesOnLevel(root, level);
	}

	void Push(TTreeItem *&root, Rhombus &obj) {
		Push(root, obj);
	}

	void Pop(int value){
		Pop(&root, value);
	}
	friend std::ostream &operator<<(std::ostream &os, const TTree &tree);

	virtual ~TTree();

private:
	void Push(TTreeItem *&item, Rhombus &rhombus);
	void Pop(TTreeItem **item, int value);
	int GetNodesOnLevel(TTreeItem* item, int level);
	TTreeItem* root;
};
#endif /* TREE_H */