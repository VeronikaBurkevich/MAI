#include "tree.h"
#include "tree_item.h"
#include "figure.h"
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include <memory>
#include <iostream>
#include <string>

int main()
{
	tree<Figure> basic;
	std::shared_ptr<Figure> ptr;
	std::shared_ptr<tree_item<Figure>> test;
	std::string action;

	std::cout << "'q' or 'quit' - exit the program." << std::endl;
	std::cout << "'d' or 'destroy' - delete the tree." << std::endl;
	std::cout << "'p' or 'print' - output the tree." << std::endl;
	std::cout << "'r' or 'remove' - remove the figure with key." << std::endl;
	std::cout << "'i' or 'insert' - insert a figure into the tree." << std::endl;
	std::cout << "'it' or 'iterator' - iterator." << std::endl;
	std::cout << "'h 'or' help '- display the help." << std::endl;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.sync();
		std::cin >> action;

		if (action == "q" || action == "quit") {
			break;
		}
		else if (action == "insert" || action == "i") {
			std::cout << "Choose figure: rhombus(1), pentagon(2), hexagon(3)." << std::endl;
			int fig;
			int new_key, father_key;
			size_t side_a, side_b;
			std::cin >> fig;
			switch (fig)
			{
			case 1: //rhombus
				std::cout << "Enter two diagonales, key and father key" << std::endl;
				std::cin >> side_a >> side_b >> new_key >> father_key;
				basic.insert(std::shared_ptr<tree_item<Figure>>(new tree_item<Figure>(std::shared_ptr<Rhombus>(new Rhombus(side_a, side_b)), new_key)), father_key);
				ptr = std::make_shared<Rhombus>(side_a, side_b);
				break;
			case 2: //pentagon
				std::cout << "Enter side, key and father key" << std::endl;
				std::cin >> side_a >> new_key >> father_key;
				basic.insert(std::shared_ptr<tree_item<Figure>>(new tree_item<Figure>(std::shared_ptr<Pentagon>(new Pentagon(side_a)), new_key)), father_key);
				ptr = std::make_shared<Pentagon>(side_a);
				break;
			case 3: //hexagon
				std::cout << "Enter side, key and father key" << std::endl;
				std::cin >> side_a >> new_key >> father_key;
				basic.insert(std::shared_ptr<tree_item<Figure>>(new tree_item<Figure>(std::shared_ptr<Hexagon>(new Hexagon(side_a)), new_key)), father_key);
				break;
			}
		}
		else if (action == "remove" || action == "r") {
			std::cout << "Enter number of figure" << std::endl;
			int key;
			std::cin >> key;
			basic.remove(key);
		}
		else if (action == "print" || action == "p") {
			if (!basic.empty()) {
				basic.Print();
			}
			else {
				std::cout << "Tree was empty" << std::endl;
			}
		}
		else if (action == "destroy" || action == "d") {
			basic.remove(1);
		}
		else if (action == "iterator" || action == "it") {
			if (!basic.empty()) {
				//basic.Print();
				for (auto i : basic) {
					i->Print();
					std::cout << std::endl;

				}
			}
			else
				std::cout << "Tree is empty" << std::endl;
		}
	
		else if (action == "help" || action == "h") {
			std::cout << "'q' or 'quit' - exit the program." << std::endl;
			std::cout << "'r' or 'remove s' - remove the figure with key." << std::endl;
			std::cout << "'d' or 'destroy' - delete the tree." << std::endl;
			std::cout << "'p' or 'print' - output the tree." << std::endl;
			std::cout << "'i' or 'insert s' - insert a figure into the tree." << std::endl;
			std::cout << "'it' or 'iterator' - iterator." << std::endl;
			std::cout << "'h 'or' help '- display the help." << std::endl;
		}
		action = " ";
	}

	//delete basic;
	system("pause");
	return 0;

}