#include <cstdlib>
#include "Rombus.h"
#include "Pentagon.h"
#include "Hexagon.h"

void option(Figure *figure) {
    figure->Print();
    std::cout << "SQUARE: " << figure->SQUARE() << std::endl;
    delete figure;
}

int print_help()
{
    int variant;
    std::cout << "Menu\n" << std::endl;
    std::cout << "1. Beginning of work\n"
              << "2. Rombus\n"
              << "3. Pentagon\n"
              << "4. Hexagon\n"
              << "5. Help\n"
              << "6. Exit\n" << std::endl;
    std::cout << ">>> ";
    std::cin >> variant;
    return variant;
}

int main(int argc, char* argv[])
{
    int work = 1;
    print_help();
    int cmd;
    while (work && scanf("%d", &cmd) == 1) {
    switch(cmd) {
        case 2:
            std::cout << "the lengths of the first diagonal and the second diagonal are " << std::endl;
            option(new Rombus(std::cin));
            break;
        case 3:
            std::cout << "the length of each side is " << std::endl;
            option(new Pentagon(std::cin));
            break;
        case 4:
            std::cout << "the length of each side is " << std::endl;
            option(new Hexagon(std::cin));
            break;
        case 5:
            print_help();
            break;   
        case 6:
            std::cout << "EXIT" << std::endl;
            exit(EXIT_SUCCESS);
            break;
        default:
            std::cerr << "Error" << std::endl;
            exit(EXIT_FAILURE);
    }
}
    return 0;
}


