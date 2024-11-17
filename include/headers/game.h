#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "card.h"
#include "gameboard.h"

class Game {
private:
    GameBoard gameBoard;
    sf::RenderWindow questionWindow;

    Card* firstFlippedCard = nullptr;
    Card* secondFlippedCard = nullptr;
    bool isCheckingMatch = false;


public:
    void handleMatch();
    void openQuestionWindow();
    void run();
    void processEvents();
    void update();
    void render();
};

#endif // GAME_H
