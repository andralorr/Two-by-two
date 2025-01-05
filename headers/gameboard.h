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
    std::vector<std::string> currentRoundAnimals;

public:
    GameBoard();
    GameBoard(const GameBoard& other);
    GameBoard& operator=(const GameBoard& other);
    ~GameBoard();

    void initializeCards();
    void positionCards();
    void render();
    void shuffleCards();
    const std::vector<std::string>& getCurrentRoundAnimals() const { return currentRoundAnimals;}
    sf::RenderWindow& getWindow();
    Card* getCardAtPosition(sf::Vector2i position);
    friend std::ostream& operator<<(std::ostream& os, const GameBoard& gameBoard);
};
#endif //GAMEBOARD_H