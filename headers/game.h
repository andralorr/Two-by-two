#pragma once
#include "../headers/gameboard.h"
#include "../headers/gameboardquiz.h"
#include "../headers/question.h"
#include <SFML/Graphics.hpp>

class Game {
private:
    GameBoard gameBoard;
    GameBoardQuiz gameBoardQuiz;
    Card* firstFlippedCard = nullptr;
    Card* secondFlippedCard = nullptr;
    Question* currentQuestion = nullptr;
    Question question;
    bool isCheckingMatch = false;
    bool isQuizActive = false;
    float matchDelay = 1.0f;
    sf::Clock matchTimer;
public:
    Game();
    void run();
    void processEvents();
    void update();
    void render();
    void handleMatch();
    void openQuestionWindow();
};