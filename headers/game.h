#pragma once

#include "gameboard.h"
#include "gameboardquiz.h"
#include "timer.h"
#include "gamemessage.h"
#include <vector>
#include <memory>

class Game {
private:
    GameBoard gameBoard;
    GameBoardQuiz gameBoardQuiz;
    Timer timer;
    bool isQuizActive;
    bool isGameOver;

    Card* firstFlippedCard = nullptr;
    Card* secondFlippedCard = nullptr;
    std::unique_ptr<IQuestion> currentQuestion;
    bool isCheckingMatch = false;

    std::vector<std::unique_ptr<IGameMessage>> messages;
    sf::Clock matchTimer;
    const float matchDelay = 0.3f;
    std::vector<std::string> correctlyAnsweredAnimals;

    std::unique_ptr<Card> copiedFirstCard;

public:
    Game();
    void run();
    void update();
    void render();
    void restartGame();

    void addMessage(const std::string& type, const std::string& assetPath);
    void displayMessages();
    void processEvents();
    void processGameBoardEvents();
    void processQuizEvents();
    void handleCardSelection(sf::Event::MouseButtonEvent mouseButton);

    static void handleWindowClose(sf::RenderWindow& window);
    void handleQuizWindowClose();
    void handleQuizOptionSelection(sf::Event::MouseButtonEvent mouseButton);
    void handleMatch();
    void openQuestionWindow();
    void handleCorrectAnswer();
    void resetGameAfterWrongAnswer();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    bool allQuestionsAnsweredCorrectly();
};
