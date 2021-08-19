#include <cstdlib>
#include <iostream>
#include <string>
#include "Pentagon.h"
#include "Rhombus.h"
#include "Hexagon.h"
#include "tree_item.h"
#include "tree.h"
#include <memory>

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
	std::cout << "8) Sort demo" << std::endl;
	std::cout << "9) Parallel sort demo" << std::endl;
	std::cout << "0) Help" << std::endl;
	while (input != 6) {
		std::cin >> input;
		if (input == 1) {
			std::cout << "Insert diagonales, key, parent key" << std::endl;
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