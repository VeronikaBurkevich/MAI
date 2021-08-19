#include "binary_tree.h"

using namespace std;

template <class T>
void TBinaryTree<T>::rebuild()
{
	for (int i = 0; i < 4; ++i) {
		if (bt[i + 1] != nullptr && bt[i] == nullptr) {
			bt[i] = bt[i + i];
			bt[i + 1] = nullptr;
		}
	}
}


template <class T>
bool TBinaryTree<T>::empty()
{
	return size == 0;
}

template <class T>
T*TBinaryTree<T>::pop()
{
	T* tmp = bt[size - 1];
	bt[size - 1] = nullptr;
	size--;
	return tmp;
}

template <class T>
void TBinaryTree<T>::push(T *value)
{
	bt[size++] = value;
	Sort();
}


template <class T>
int TBinaryTree<T>::Size()
{
	return size;
}

template <class T>
TBinaryTree<T>::TBinaryTree()
{
	for (int i = 0; i < 5; ++i) {
		bt[i] = nullptr;
	}
	size = 0;
}

template <class T>
void TBinaryTree<T>::Sort()
{
	int item;
	T *temp;
	for (int i = 0; i < size; ++i) {
		temp = bt[i];
		item = i - 1;
		while (item >= 0 && bt[item]->Square() > temp->Square())
		{
			bt[item + 1] = bt[item];
			bt[item] = temp;
			item--;
		}
	}
}


template<class A>
std::ostream& operator<<(std::ostream& os, const TBinaryTree<A >& obj)
{
	for (int i = 0; i < obj.size; ++i) {
		std::cout << "     Tree element " << i << " is ";
		//cout << *(obj.bt[i]);
		(*(obj.bt[i])).Print();
	}
	return os;
}


template<class A>
std::istream& operator >> (std::istream& is, const TBinaryTree<A>& obj)
{
	int size = 0;
	std::cout << "Cin size:\n";
	std::cin >> size;
	Figure rt;
	for (int i = 0; i < size; ++i) {
		std::cout << "Cin Figure " << i << ":\n";
		std::cin >> rt;
		obj.bt[i] = rt;
	}
}


template <class T>
TBinaryTree<T>::~TBinaryTree(){}


#include "tree.h"
#include "Figure.h"
template class tree<TBinaryTree<Figure>, Figure>;
template std::ostream& operator<<(std::ostream &os, const tree<TBinaryTree<Figure>> &list);

template class TBinaryTree<Figure>;
template std::ostream& operator<<(std::ostream &os, const TBinaryTree<Figure> &TBinaryTree);