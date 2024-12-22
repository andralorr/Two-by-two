#include "../headers/game.h"
#include "../headers/question.h"
#include <iostream>

Game::Game(): timer(50) {
    question.initializeQuestions();
    isQuizActive = false;
    currentQuestion = nullptr;
    isGameOver = false;
}

void Game::run() {
    StartMessage startMessage(gameBoard.getWindow());
    startMessage.display();

    while (gameBoard.getWindow().isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    if (!isQuizActive)
        processGameBoardEvents();
    else
        processQuizEvents();
}

void Game::processGameBoardEvents() {
    sf::Event event{};
    while (gameBoard.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            handleWindowClose(gameBoard.getWindow());
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            handleCardSelection(event.mouseButton);
        }
    }
}

void Game::handleWindowClose(sf::RenderWindow& window) {
    window.close();
}

void Game::handleCardSelection(sf::Event::MouseButtonEvent mouseButton) {
    if (mouseButton.button == sf::Mouse::Left) {
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

void Game::processQuizEvents() {
    sf::Event event{};
    while (gameBoardQuiz.getWindowQuiz().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            handleQuizWindowClose();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            handleQuizOptionSelection(event.mouseButton);
        }
    }
}

void Game::handleQuizWindowClose() {
    gameBoardQuiz.getWindowQuiz().close();
    isQuizActive = false;
}

void Game::handleQuizOptionSelection(sf::Event::MouseButtonEvent) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(gameBoardQuiz.getWindowQuiz());
    int clickedOption = gameBoardQuiz.getOptionAtPosition(mousePosition);

    if (clickedOption != -1) {
        if (currentQuestion) {
            std::cout << *currentQuestion << std::endl;
            if (currentQuestion->checkAnswer(clickedOption)) {
                std::cout << "Correct answer!" << std::endl;
                answeredQuestions.push_back(currentQuestion);
                handleCorrectAnswer();
            } else {
                std::cout << "Wrong answer!" << std::endl;
                resetGameAfterWrongAnswer();
            }
        } else {
            std::cerr << "Error: currentQuestion is null!" << std::endl;
        }
    }
}

void Game::handleCorrectAnswer() {
    gameBoardQuiz.getWindowQuiz().close();
    isQuizActive = false;
    currentQuestion = nullptr;
}

void Game::resetGameAfterWrongAnswer() {
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

void Game::update() {
    timer.update();

    if (timer.isTimeUp()) {
        if (isQuizActive)
            gameBoardQuiz.getWindowQuiz().close();

        FailureMessage failureMessage(gameBoard.getWindow());
        failureMessage.display();

        if (failureMessage.isRestartClicked()) {
            restartGame();
            return;
        }
    }

    if (allQuestionsAnsweredCorrectly()) {
        SuccessMessage successMessage(gameBoard.getWindow());
        successMessage.display();
        return;
    }

    if (isCheckingMatch && !isQuizActive) {
        if (matchTimer.getElapsedTime().asSeconds() >= matchDelay)
            handleMatch();
    }
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
    gameBoard.getWindow().clear();

    gameBoard.render();

    timer.render(gameBoard.getWindow());

    if (isQuizActive) {
        gameBoardQuiz.render();
    }
    gameBoard.getWindow().display();

}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "\n GameBoard: " << game.gameBoard << "\n";
    return os;
}

bool Game::allQuestionsAnsweredCorrectly() {
    std::vector<Question*> allQuestions;
    for (auto& card : gameBoard.getCards()) {
        Question* question = Question::getQuestionForAnimal(card.getAnimal());
        if (question && std::find(allQuestions.begin(), allQuestions.end(), question) == allQuestions.end()) {
            allQuestions.push_back(question);
        }
    }

    for (auto& question : allQuestions) {
        if (std::find(answeredQuestions.begin(), answeredQuestions.end(), question) == answeredQuestions.end()) {
            return false;
        }
    }

    return true;
}

void Game::restartGame() {
    isQuizActive = false;
    isGameOver = false;
    timer.reset(50);
    firstFlippedCard = nullptr;
    secondFlippedCard = nullptr;
    isCheckingMatch = false;
    answeredQuestions.clear();
    gameBoard.shuffleCards();
    for (auto& card : gameBoard.getCards()) {
        card.setMatched(false);
        if (card.is_flipped()) {
            card.flip();
        }
    }
    Question::initializeQuestions();

    std::cout << "Game has been restarted!" << std::endl;
}