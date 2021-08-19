#include "tree.h"

template <class T>
class TBinaryTree
{
public:
	TBinaryTree();
	void rebuild();
	//void push(T * value);
	std::shared_ptr<TBinaryTree<T>> push(std::shared_ptr<T> figure);
	int Size();
	void Sort();
	bool empty();
	T* pop();
	template <class A> friend std::ostream& operator<<(std::ostream& os, const TBinaryTree<A> & obj);
	template <class A> friend std::istream& operator >> (std::istream& is, const TBinaryTree<A>& obj);
	~TBinaryTree();
	//private:
		//TBinaryTree *left;
		//TBinaryTree *right;
		//T data;
	T* bt[5];
	int size;
};