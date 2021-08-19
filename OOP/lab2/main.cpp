#include <cstdlib>
#include <iostream>
#include "Rhombus.h"
#include "Tree.h"
#include "Item1.h"

void help()
{
	std::cout << "1 - меню" << std::endl;
	std::cout << "2 - добавить фигуру в дерево" << std::endl;
	std::cout << "3 - вывести количесво узлов на уровне" << std::endl;
	std::cout << "4 - печать дерева" << std::endl;
	std::cout << "5 - удалить фигуру из дерева" << std::endl;
	//std::cout << "Press 0 to exit" << std::endl;
}

double option(Figure *figure){
	//figure->Print();
	//std::cout << "SQUARE: " << figure->SQUARE() << std::endl;
	double square;
	square = figure->SQUARE();
	delete figure;
	return square;
}

int main(int argc, const char * argv[]) {
	help();

	size_t act;
	TTree s;

	while (true) {
		std::cin >> act;
		//if (act == 0) break;
		if (act > 6) {
			std::cout << "Error: enter another parameter" << std::endl;
			continue;
		}
		switch (act) {
		case 1: {
			help();
			break;
		}
		case 2: {
			std::cout << "Adding the Rhombus" << std::endl;
			std::cout << "The lengths of the first diagonal and the second diagonal are " << std::endl;
			Rhombus obj(std::cin);
			//Rhombus obj2(std::cin);
			s.Push(Rhombus SQUARE());
			break;
		}
		case 3: {
			int number = 0;
			std::cout << "Enter level to get a number of nodes" << std::endl;
			std::cin >> number;
			std::cout << "On level " << number << " nodes: " << s.GetNodesOnLevel(number) << std::endl;
			break;
		}
		case 4: {
			std::cout << s << std::endl;
			break;
		}
		case 5: {
			std::cout << "Enter side of Rhombus to delete him: " << std::endl;
			int value;
			std::cin >> value;
			s.Pop(value);
			break;
		}
		default: {
			std::cout << "Error, incorrect data" << std::endl;
			break;
		//}
		}
	}
	return 0;
}