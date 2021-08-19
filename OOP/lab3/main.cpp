#include "Figure.h"
#include "tree.h"
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include <memory>
#include <iostream>
#include <string>

int main()
{
	tree* basic = new tree();
	std::string action;

	std::cout << "'q' or 'quit' - exit the program." << std::endl;
	std::cout << "'d' or 'destroy' - delete the tree." << std::endl;
	std::cout << "'p' or 'print' - output the tree." << std::endl;
	std::cout << "'r' or 'remove' - remove the Figure with key." << std::endl;
	std::cout << "'i' or 'insert' - insert a Figure into the tree." << std::endl;
	std::cout << "'h 'or' help '- display the help." << std::endl;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.sync();
		std::cin >> action;

		if (action == "q" || action == "quit") {
			break;
		}
		else if (action == "insert" || action == "i") {
			std::cout << "Choose Figure: rhombus(1), hexagon(2), pentagon(3)." << std::endl;
			int fig;
			int new_key, father_key;
			size_t side_d1, side_d2;
			std::cin >> fig;
			switch (fig)
			{
			case 1: //rhombus
				std::cout << "Enter two diagonales, key and father key" << std::endl;
				std::cin >> side_d1 >> side_d2 >> new_key >> father_key;
				basic->insert(std::shared_ptr<tree_item>(new tree_item(std::shared_ptr<rhombus>(new rhombus(side_d1, side_d2)), new_key)), father_key);
				break;
			case 2: //hexagon
				std::cout << "Enter side, key and father key" << std::endl;
				std::cin >> side_d1 >> new_key >> father_key;
				basic->insert(std::shared_ptr<tree_item>(new tree_item(std::shared_ptr<hexagon>(new hexagon(side_d1)), new_key)), father_key);
				break;
			case 3: //pentagon
				std::cout << "Enter side, key and father key" << std::endl;
				std::cin >> side_d1 >> new_key >> father_key;
				basic->insert(std::shared_ptr<tree_item>(new tree_item(std::shared_ptr<pentagon>(new pentagon(side_d1)), new_key)), father_key);
				break;
			}
		}
		else if (action == "remove" || action == "r") {
			std::cout << "Enter number of Figure" << std::endl;
			int key;
			std::cin >> key;
			basic->remove(key);
		}
		else if (action == "print" || action == "p") {
			if (!basic->empty()) {
				basic->print();
			}
			else {
				std::cout << "Tree was empty" << std::endl;
			}
		}
		else if (action == "destroy" || action == "d") {
			delete basic;
		}
		else if (action == "help" || action == "h") {
			std::cout << "'q' or 'quit' - exit the program." << std::endl;
			std::cout << "'r' or 'remove' - remove the Figure with key." << std::endl;
			std::cout << "'d' or 'destroy' - delete the tree." << std::endl;
			std::cout << "'p' or 'print' - output the tree." << std::endl;
			std::cout << "'i' or 'insert' - insert a Figure into the tree." << std::endl;
			std::cout << "'h 'or' help '- display the help." << std::endl;
		}
		action = " ";

	}

	delete basic;
	system("pause");
	return 0;

}
