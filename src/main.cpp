#include "Controller.h"
#include <iostream>

int main() {
    try {
        Controller game;
        game.Run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << '\n';

        std::cout << "Press Enter to exit...";
        std::cin.get();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}