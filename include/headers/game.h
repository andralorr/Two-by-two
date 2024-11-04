#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "card.h"
#include "question.h"
#include <memory>

class Game {
private:

    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture backTexture;
    std::vector<std::shared_ptr<sf::Texture>> frontTextures;    std::vector<Card> cards;

    //std::vector<Question> questions;
    sf::Clock clock;
    sf::Time timeLimit;

    Card* firstFlippedCard = nullptr;
    Card* secondFlippedCard = nullptr;

    Card* getCardAtPosition(sf::Vector2i position);
    bool isCheckingMatch = false;
    sf::Clock flipClock;
public:
    void processEvents();
    void update();
    void render();
    void shuffleCards();
    //void askQuestion(const Card& card);
    void positionCards();
    void initializeCards();

    Game();
    void run();
};

#endif

