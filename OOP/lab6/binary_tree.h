#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include "tree.h"


class TBinaryTree
{
public:
	TBinaryTree();
	void push(void *value);
	int Size();
	~TBinaryTree();
	//private:
	TBinaryTree *left;
	TBinaryTree *right;
	//T data;
	void *data;
	int size;
};
#endif // !BINARY_TREE_H
