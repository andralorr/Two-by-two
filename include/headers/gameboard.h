#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Card.h"
#include "Question.h"

class GameBoard {
private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture backTexture;
    std::vector<std::shared_ptr<sf::Texture>> frontTextures;

    std::vector<Card> cards;
    std::vector<Question> questions;
    Question* currentQuestion = nullptr;

    sf::Font font;
    sf::Text questionText;
    std::vector<sf::Text> optionTexts;
    std::vector<sf::RectangleShape> optionBoxes;

public:
    GameBoard();
    void initializeCards();
    void positionCards();
    void render();
    void shuffleCards();
    sf::RenderWindow& getWindow();
    Card* getCardAtPosition(sf::Vector2i position);

    const std::vector<sf::RectangleShape>& getOptionBoxes() const;
    void setupQuestion(const Question& question);
    void drawQuestion(const Question& question);
};
#endif //GAMEBOARD_H
