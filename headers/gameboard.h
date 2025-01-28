#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../headers/card.h"
#include "../headers/singleton.h"

class GameBoard : public Singleton<GameBoard> {
    friend class Singleton<GameBoard>;

private:
    sf::RenderWindow cardsWindow;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture backTexture;
    std::vector<std::shared_ptr<sf::Texture>> frontTextures;

    std::vector<Card> cards;
    std::vector<std::string> currentRoundAnimals;

    GameBoard();
    ~GameBoard();

    friend std::ostream& operator<<(std::ostream& os, const GameBoard& gameBoard);
public:
    sf::RenderWindow& getWindow();
    Card* getCardAtPosition(const sf::Vector2i& position);
    void initializeCards();
    void positionCards();
    void shuffleCards();
    void render();
    const std::vector<std::string>& getCurrentRoundAnimals() const {return currentRoundAnimals;}
};

#endif //GAMEBOARD_H
