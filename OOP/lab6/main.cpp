#include <cstdlib>
#include <iostream>
#include <string>
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "tree_item.h"
#include "tree.h"
#include <memory>



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
	tree<Figure> tree;
	std::shared_ptr<tree_item<Figure>> test;
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
	std::cout << "Insert: sides, key, parent key" << std::endl;
	while (input != 6) {
		std::cin >> input;
		if (input == 1) {
			std::cout << "Insert two diagonales, key, parent key" << std::endl;
			ptr = std::make_shared<Rhombus>(std::cin);
			std::cin >> key >> parentKey;
			tree.insert(ptr, parentKey, key);
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
			//if (!tree.empty()) {
				std::cout << "Enter number of figure" << std::endl;
				size_t key;
				std::cin >> key;
				tree.remove(key);
			//}

			//else 
				//std::cout << "Tree is empty." << std::endl;

		}
		else if (input == 5) {
			if (!tree.empty())
				tree.print();
			else {
				std::cout << "Tree is empty." << std::endl;
			}
		}
		else if (input == 7) {
			if (tree.empty())
				std::cout << "Tree is empty" << std::endl;
			else {
				for (auto i : tree) {
					i->Print();
					//std::cout << std::endl;
				}
			}
		}
	}
	system("pause");
	return 0;
}