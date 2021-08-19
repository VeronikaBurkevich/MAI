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
	tree<figure> basic;
	std::shared_ptr<figure> ptr;
	std::string action;

	std::cout << "'q' or 'quit' - exit the program." << std::endl;
	std::cout << "'d' or 'destroy' - delete the tree." << std::endl;
	std::cout << "'p' or 'print' - output the tree." << std::endl;
	std::cout << "'r' or 'remove' - remove the figure with key." << std::endl;
	std::cout << "'i' or 'insert' - insert a figure into the tree." << std::endl;
	std::cout << "'h 'or' help '- display the help." << std::endl;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.sync();
		std::cin >> action;

		if (action == "q" || action == "quit") {
			break;
		}
		else if (action == "insert" || action == "i") {
			std::cout << "Choose figure: rhombus(1), hexagon(2), pentagon(3)." << std::endl;
			int fig;
			int new_key, father_key;
			size_t side_a, side_b;
			std::cin >> fig;
			switch (fig)
			{
			case 1: //rhombus
				std::cout << "Enter two diagonales, key and father key" << std::endl;
				std::cin >> side_a >> side_b >> new_key >> father_key;
				basic.insert(std::shared_ptr<tree_item<figure>>(new tree_item<figure>(std::shared_ptr<rhombus>(new rhombus(side_a, side_b)), new_key)), father_key);
				ptr = std::make_shared<rhombus>(side_a, side_b);
				break;
			case 2: //hexagon
				std::cout << "Enter side, key and father key" << std::endl;
				std::cin >> side_a >> new_key >> father_key;
				basic.insert(std::shared_ptr<tree_item<figure>>(new tree_item<figure>(std::shared_ptr<hexagon>(new hexagon(side_a)), new_key)), father_key);
				ptr = std::make_shared<hexagon>(side_a);
				break;
			case 3: //pentagon
				std::cout << "Enter side, key and father key" << std::endl;
				std::cin >> side_a >> new_key >> father_key;
				basic.insert(std::shared_ptr<tree_item<figure>>(new tree_item<figure>(std::shared_ptr<pentagon>(new pentagon(side_a)), new_key)), father_key);
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
				basic.print();
			}
			else {
				std::cout << "Tree was empty" << std::endl;
			}
		}
		/*else if (action == "destroy" || action == "d") {
			basic.remove();
		}*/
		else if (action == "help" || action == "h") {
			std::cout << "'q' or 'quit' - exit the program." << std::endl;
			std::cout << "'r' or 'remove s' - remove the figure with key." << std::endl;
			std::cout << "'d' or 'destroy' - delete the tree." << std::endl;
			std::cout << "'p' or 'print' - output the tree." << std::endl;
			std::cout << "'i' or 'insert s' - insert a figure into the tree." << std::endl;
			std::cout << "'h 'or' help '- display the help." << std::endl;
		}
		action = " ";

	}

	//delete basic;
	system("pause");
	return 0;

}