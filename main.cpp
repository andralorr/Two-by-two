#include "headers/game.h"
#include "headers/gameexception.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    } catch (const GameException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}