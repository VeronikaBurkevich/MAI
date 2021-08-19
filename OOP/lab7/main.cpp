#include <iostream>
#include "tree.h"
#include <string>
#include "Figure.h"
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "binary_tree.h"

using namespace std;


void TestAllocationBlock()
{
	TAllocationBlock allocator(sizeof(int), 10);
	int *a1 = nullptr;
	int *a2 = nullptr;
	int *a3 = nullptr;
	int *a4 = nullptr;
	int *a5 = nullptr;

	a1 = (int*)allocator.allocate(); *a1 = 1; std::cout << "a1 pointer value:"
		<< *a1 << std::endl;
	a2 = (int*)allocator.allocate(); *a2 = 2; std::cout << "a2 pointer value:"
		<< *a2 << std::endl;
	a3 = (int*)allocator.allocate(); *a3 = 3; std::cout << "a3 pointer value:"
		<< *a3 << std::endl;
	allocator.deallocate(a1);
	allocator.deallocate(a3);
	a4 = (int*)allocator.allocate(); *a4 = 4; std::cout << "a4 pointer value:"
		<< *a4 << std::endl;
	a5 = (int*)allocator.allocate(); *a5 = 5; std::cout << "a5 pointer value:"
		<< *a5 << std::endl;
	std::cout << "a1 pointer value:" << *a1 << std::endl;
	std::cout << "a2 pointer value:" << *a2 << std::endl;
	std::cout << "a3 pointer value:" << *a3 << std::endl;
	allocator.deallocate(a2);
	allocator.deallocate(a4);
	allocator.deallocate(a5);
}


int main()
{
	//tree<Figure> tree;	

	tree<TBinaryTree<Figure>> tree;
	std::shared_ptr<tree_item<TBinaryTree<Figure>>> test;
	std::shared_ptr<TBinaryTree<Figure>> bin;
	std::shared_ptr<Figure> ptr;
	size_t input = 0, key, parentKey;
	std::cout << "Choose an operation:" << std::endl;
	std::cout << "1) Add rhombus" << std::endl;
	std::cout << "2) Add pentagon" << std::endl;
	std::cout << "3) Add hexagon" << std::endl;
	std::cout << "4) Delete node from tree" << std::endl;
	std::cout << "5) Print tree" << std::endl;
	std::cout << "6) Exit" << std::endl;
	std::cout << "7) Iterator demo" << std::endl;
	std::cout << "8) Sort demo" << std::endl;
	std::cout << "9) Parallel sort demo" << std::endl;
	std::cout << "0) Help" << std::endl;
	while (input != 6) {
		std::cin >> input;
		if (input == 1) {
			std::cout << "Insert diagonales, key, parent key" << std::endl;
			ptr = std::make_shared<Rhombus>(std::cin);
			std::cin >> key >> parentKey;
			tree.insert(bin.push(ptr), parentKey, key);
		}
		else if (input == 2) {
			std::cout << "Insert side, key, parent key" << std::endl;
			ptr = std::make_shared<Pentagon>(std::cin);
			std::cin >> key >> parentKey;
			tree.insert(ptr, parentKey, key);
		}
		else if (input == 3) {
			std::cout << "Insert side, key, parent key" << std::endl;
			ptr = std::make_shared<Hexagon>(std::cin);
			std::cin >> key >> parentKey;
			tree.insert(ptr, parentKey, key);
		}
		else if (input == 4) {
			if (!tree.empty()) {
				std::cin >> key;
				tree.remove(key);
			}
			else {
				std::cout << "tree is empty." << std::endl;
			}

		}
		else if (input == 5) {
			if (!tree.empty())
				tree.print();
			else {
				std::cout << "tree is empty." << std::endl;
			}
		}
		else if (input == 7) {
			if (tree.empty())
				std::cout << "tree is empty" << std::endl;
			else {
				for (auto i : tree) {
					i->print();
					std::cout << std::endl;
				}
			}
		}
		else if (input == 8) {
			if (tree.empty())
				std::cout << "tree is empty" << std::endl;
			else {
				tree.sort();
				//tree.print();
			}
		}
		else if (input == 9) {
			if (tree.empty())
				std::cout << "tree is empty" << std::endl;
			else {
				tree.sortParallel();
				//tree.print();
			}
		}
		else if (input == 0) {
			std::cout << "1) Add rhombus" << std::endl;
			std::cout << "2) Add pentagon" << std::endl;
			std::cout << "3) Add hexagon" << std::endl;
			std::cout << "4) Delete node from tree" << std::endl;
			std::cout << "5) Print tree" << std::endl;
			std::cout << "6) Exit" << std::endl;
			std::cout << "7) Iterator demo" << std::endl;
			std::cout << "8) Sort demo" << std::endl;
			std::cout << "9) Parallel sort demo" << std::endl;
			std::cout << "0) Help" << std::endl;
			std::cin >> input;
		}
		else
			std::cin >> input;
	}
	system("pause");
	return 0;
}
int main()
{
	//cout << "Enter size of array " << endl;
	int arr_size, kindOfFigure;
	cin >> arr_size;
	tree<TBinaryTree<Figure>> tree;
	for (int i = 0; i < 3; ++i) {
		arr.ElementGet(i).SetPtr1(new TBinaryTree<Figure>);
	}
	Rectangle * rt = new Rectangle(3, 4);
	Rhombus * rb = new Rhombus(5, 90);
	Trapeze * tr = new Trapeze(2, 2, 2, 2);

	IRemoveCriteriaByValue<Figure> criteria(Rectangle(1, 1));

	char choice = 'b';
	int index;
	cout << "a to add\nc to delete elems with small elems\nd to delete\ng to get\np to print\nq to quit\n";
	while (choice != 'q') {
		cout << "Enter your choice\n" << endl;
		cin >> choice;
		switch (choice) {
		case 'a':
			cout << "Enter the index to add" << endl;
			cin >> index;
			if (index >= arr.Size()) {
				cout << "Index to high" << endl;
				break;
			}
			cout << "What are you want to insert: 1 - rectangle , 2 - rhombus, 3 - trapeze\n";
			cin >> kindOfFigure;
			if (kindOfFigure == 1) {
				cout << "Rectangle: ";
				cin >> *rt;
				Figure* rtt = new Rectangle(*rt);
				arr.InsertSubitem(rtt);
			}
			else if (kindOfFigure == 2) {
				cout << "Rhombus: ";
				cin >> *rb;
				Figure* rbb = new Rhombus(*rb);
				arr.InsertSubitem(rbb);
			}
			else if (kindOfFigure == 3) {
				cout << "Trapeze: ";
				cin >> *tr;
				Figure* trr = new Trapeze(*tr);
				arr.InsertSubitem(trr);
			}
			else {
				cout << "Wrong choice\n";
			}
			break;
		case 'd':
			cout << "Enter the index to delete" << endl;
			cin >> index;
			if (index >= arr.Size()) {
				cout << "Index to high" << endl;
				break;
			}
			arr.ElementDelete(index);
			break;
		case 'c':
			cout << "Remove elems with small square\n Enter min square\n";
			cin >> index;
			arr.RemoveSubitem(index);
			break;
		case 'g':
			cout << "Enter the index to get" << endl;
			cin >> index;
			if (index >= arr.Size()) {
				cout << "Index to high" << endl;
				break;
			}
			cout << arr.ElementGet(index);
			break;
		case 'p':
			//for (auto itere : arr) cout << *itere << endl;
			cout << arr;
			break;
		case 'q':
			return 0;
		}
	}



	return 0;
}