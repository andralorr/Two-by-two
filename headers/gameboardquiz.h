#ifndef GAMEBOARDQUIZ_H
#define GAMEBOARDQUIZ_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "../headers/question.h"
#include "../headers/singleton.h"

class GameBoardQuiz : public Singleton<GameBoardQuiz> {
    friend class Singleton<GameBoardQuiz>;

private:
    sf::RenderWindow quizWindow;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    IQuestion* currentQuestion = nullptr;

    sf::Font font;
    sf::Text questionText;
    std::vector<sf::Text> optionTexts;
    std::vector<sf::RectangleShape> optionBoxes;

    GameBoardQuiz();
    ~GameBoardQuiz();

    void positionQuestions();

public:
    sf::RenderWindow& getWindowQuiz();
    void createWindow();
    void render();
    int getOptionAtPosition(const sf::Vector2i& position) const;
    void setCurrentQuestion(IQuestion* question);
};

#endif // GAMEBOARDQUIZ_H
