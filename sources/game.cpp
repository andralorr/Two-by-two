#include "../headers/game.h"
#include "../headers/question.h"
#include <iostream>

Game::Game() {
    question.initializeQuestions();
    isQuizActive = false;
    currentQuestion = nullptr;
}

void Game::run() {
    while (gameBoard.getWindow().isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    if (!isQuizActive) {
        while (gameBoard.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameBoard.getWindow().close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(gameBoard.getWindow());
                    Card* clickedCard = gameBoard.getCardAtPosition(mousePosition);

                    if (clickedCard && !clickedCard->isMatched() && clickedCard != firstFlippedCard) {
                        clickedCard->flip();

                        if (!firstFlippedCard) {
                            firstFlippedCard = clickedCard;
                        } else {
                            secondFlippedCard = clickedCard;
                            isCheckingMatch = true;
                            matchTimer.restart();
                        }
                    }
                }
            }
        }
    }

    if (isQuizActive && gameBoardQuiz.getWindowQuiz().isOpen()) {
        while (gameBoardQuiz.getWindowQuiz().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameBoardQuiz.getWindowQuiz().close();
                isQuizActive = false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(gameBoardQuiz.getWindowQuiz());
                int clickedOption = gameBoardQuiz.getOptionAtPosition(mousePosition);

                if (clickedOption != -1) {
                    if (currentQuestion && currentQuestion->checkAnswer(clickedOption)) {
                        std::cout << "Correct answer!" << std::endl;
                        gameBoardQuiz.getWindowQuiz().close();
                        isQuizActive = false;
                        currentQuestion = nullptr;
                    } else {
                        std::cout << "Wrong answer." << std::endl;
                        gameBoardQuiz.getWindowQuiz().close();
                        isQuizActive = false;
                        currentQuestion = nullptr;
                        gameBoard.shuffleCards();
                        for (auto& card : gameBoard.getCards()) {
                            card.setMatched(false);
                            if (card.is_flipped()) {
                                card.flip();
                            }
                        }
                        firstFlippedCard = nullptr;
                        secondFlippedCard = nullptr;
                        isCheckingMatch = false;
                    }
                    gameBoardQuiz.getWindowQuiz().close();
                    isQuizActive = false;
                    currentQuestion = nullptr;
                }
            }
        }
    }
}

void Game::update() {
    if (isCheckingMatch && !isQuizActive) {
        if (matchTimer.getElapsedTime().asSeconds() >= matchDelay) {
            handleMatch();
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << "Card: " << card.getAnimal() << " | Matched: " << (card.isMatched()? "Yes" : "No");
    return os;
}

void Game::handleMatch() {
    if (firstFlippedCard && secondFlippedCard) {
        std::cout << "Checking cards:" << std::endl;
        std::cout << *firstFlippedCard << std::endl;
        std::cout << *secondFlippedCard << std::endl;
        if (firstFlippedCard->getAnimal() == secondFlippedCard->getAnimal()) {
            currentQuestion = Question::getQuestionForAnimal(firstFlippedCard->getAnimal());

            if (currentQuestion != nullptr) {
                firstFlippedCard->setMatched(true);
                secondFlippedCard->setMatched(true);
                openQuestionWindow();
            } else {
                std::cerr << "Error: No question associated with the matched animal." << std::endl;
            }
        } else {
            firstFlippedCard->flip();
            secondFlippedCard->flip();
        }

        firstFlippedCard = nullptr;
        secondFlippedCard = nullptr;
        isCheckingMatch = false;
    }
}

void Game::openQuestionWindow() {
    if (currentQuestion != nullptr) {
        gameBoardQuiz.createWindow();
        gameBoardQuiz.setCurrentQuestion(currentQuestion);
        isQuizActive = true;
    }
}

void Game::render() {
    gameBoard.render();
    if (isQuizActive) {
        gameBoardQuiz.render();
    }
}