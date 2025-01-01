#include "headers/game.h"
#include "headers/gameexception.h"
#include <iostream>
#include <fstream>
int main() {
    try {
        QuestionFactory::loadQuestionsFromFile("Init/questions.txt", "Init/truefalse.txt");
        std::cout << "Questions loaded successfully.\n";
        std::cout << "Game starting...\n";
        Game game;
        std::cout << "Game initialized successfully.\n";
        game.run();
    } catch (const GameException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
