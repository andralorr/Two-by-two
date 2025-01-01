#include "headers/game.h"
#include "headers/gameexception.h"
#include <iostream>
#include <fstream>
int main() {
    try {
        std::cout << "Game starting...\n";
        Game game;
        std::cout << "Game initialized successfully.\n";
        game.run();
    } catch (const GameException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}