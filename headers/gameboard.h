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
    GameBoard(const GameBoard& other);
    GameBoard& operator=(const GameBoard& other);
    ~GameBoard();

    void initializeCards();
    void positionCards();
    void render();
    void shuffleCards();
    std::vector<Card>& getCards();

    sf::RenderWindow& getWindow();
    Card* getCardAtPosition(sf::Vector2i position);
    friend std::ostream& operator<<(std::ostream& os, const GameBoard& gameBoard);
};
#endif //GAMEBOARD_H
