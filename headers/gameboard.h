#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../headers/card.h"

class GameBoard {
private:
    sf::RenderWindow cardsWindow;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture backTexture;
    std::vector<std::shared_ptr<sf::Texture>> frontTextures;

    std::vector<Card> cards;

public:
    GameBoard();
    void initializeCards();
    void positionCards();
    void render();
    void shuffleCards();
    sf::RenderWindow& getWindow();
    Card* getCardAtPosition(sf::Vector2i position);
};
#endif //GAMEBOARD_H
