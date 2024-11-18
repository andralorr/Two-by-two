#ifndef GAMEBOARDQUIZ_H
#define GAMEBOARDQUIZ_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "../headers/question.h"

class GameBoardQuiz {
private:
    sf::RenderWindow quizWindow;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    Question* currentQuestion = nullptr;

    sf::Font font;
    sf::Text questionText;
    std::vector<sf::Text> optionTexts;
    std::vector<sf::RectangleShape> optionBoxes;
public:
    GameBoardQuiz();
    void createWindow();
    void setCurrentQuestion(Question* question);
    void positionQuestions();
    void render();
    sf::RenderWindow& getWindowQuiz();
    int getOptionAtPosition(const sf::Vector2i& position) const;
};
#endif //GAMEBOARDQUIZ_H
