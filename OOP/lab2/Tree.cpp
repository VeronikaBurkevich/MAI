#include <iostream>
#include "Tree.h"
#include "Item1.h"
#include "Rhombus.h"

TTree::TTree() : root(nullptr) {
}

TTree::TTree(const TTree &orig) {
	root = orig.root;
}

void TreeDel(TTreeItem* item) {  // ���������� ��� ������
	if (item) {
		TreeDel(item->GetBrother());
		TreeDel(item->GetSon());
		delete item;
	}
}

TTree::~TTree() {
	TreeDel(root);
}

int TTree::GetNodesOnLevel(TTreeItem *item, int level) {    //������� ������������, ������� ��������� �� ������������ ������
	if (item == nullptr) {
		return 0;
	}

	if (level <= 0) {
		return 0;
	}

	return GetNodesOnLevel(item->GetSon(), level - 1) + (level == 1) + GetNodesOnLevel(item->GetBrother(), level);
}

void TTree::Push(TTreeItem *&item, Rhombus &Rhombus) {  //������� ���������� �������� � ������
	if (item == nullptr) {
		item = new TTreeItem(Rhombus);
	}
	else if (Rhombus.SQUARE() < item->GetRhombus().SQUARE()) {
		Push(item->GetSon(), Rhombus);
	}
	else {
		Push(item->GetBrother(), Rhombus);
	}

}

TTreeItem *tree_find_parent(TTreeItem *item, int value) { //������� �� ������ ������ �� ��������
	if (item == nullptr) {
		return nullptr;
	}
	if (item->GetRhombus().SQUARE() == value) {
		return item;
	}
	TTreeItem *parent_node = tree_find_parent(item->GetSon(), value);
	if (parent_node == nullptr) {
		parent_node = tree_find_parent(item->GetBrother(), value);
	}
	return parent_node;
}

int tree_find_parent1(TTreeItem **item, int value) { // ������� ������ ������ �� ��������� �� ��������
	int idx = 0;
	if ((*item)->GetRhombus().SQUARE() == value) {
		return 1;
	}
	if ((*item)->GetSon() != nullptr){
		idx = tree_find_parent1(&((*item)->GetSon()), value);
	}
	if ((*item)->GetBrother() != nullptr){
		idx = tree_find_parent1(&((*item)->GetBrother()), value);
	}

	if (idx == 1) {
		delete((*item)->GetBrother());
		(*item)->GetBrother() = nullptr;
		idx = 0;
	}
	return 0;
}

void split(TTreeItem **item) {  //������� ��� ���������������� ����� (��������)
	TTreeItem *tmp = (*item)->GetBrother();
	(*item)->GetSon() = tmp->GetSon();
	(*item)->GetRhombus() = tmp->GetRhombus();
	(*item)->GetBrother() = tmp->GetBrother();
	delete(tmp);
	tmp = nullptr;
}

TTreeItem *destroy_Tree(TTreeItem **pointer) {
	if ((*pointer) == nullptr) {
		return nullptr;
	}

	if ((*pointer)->GetSon() != nullptr) {
		destroy_Tree(&((*pointer)->GetSon()));
	}

	if ((*pointer)->GetBrother() != nullptr) {
		destroy_Tree(&((*pointer)->GetBrother()));
	}

	if ((*pointer)->GetSon() == nullptr && (*pointer)->GetBrother() == nullptr) {
		delete((*pointer));
		*pointer = nullptr;
	}
	return nullptr;
}

void TTree::Pop(TTreeItem **item, int value) {  //������� �������� �������� �� ������
	TTreeItem *pointer = tree_find_parent(*item, value);
	destroy_Tree(&(pointer->GetSon()));
	if (pointer->GetBrother() != nullptr) {
		split(&pointer);
	}
	else if (pointer->GetBrother() == nullptr) {
		if ((*item)->GetSon() == nullptr) {
			delete(*item);
			*item = nullptr;
		}
		else {
			TTreeItem **son = &((*item)->GetSon());
			if ((*son)->GetRhombus().SQUARE() == value) {
				delete(*son);
				*son = nullptr;
			}
			else {
				tree_find_parent1(item, value);
			}
		}
	}
}


void TSpace(size_t n){ // ������� ������������� �������
	for (size_t i = 0; i <= n; i++)
		std::cout << "    ";
}

void TreeRun(std::ostream &os, TTreeItem *item, size_t space){ //������� ������ ������ ��� ������
	if (item) {
		TSpace(space);
		os << *item << std::endl;
		if (item->GetBrother() != nullptr) {
			TreeRun(os, item->GetBrother(), space);
		}
		if (item->GetSon() != nullptr) {
			TreeRun(os, item->GetSon(), space + 1);
		}

	}
}

std::ostream& operator<<(std::ostream &os, const TTree &tree) { //�������� ������ ������
	TTreeItem *obj = tree.root;
	os << "Printed tree:" << std::endl;
	TreeRun(os, obj, 1);
	return os;
}