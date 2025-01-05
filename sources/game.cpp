#include "../headers/game.h"
#include "../headers/gamemessage.h"
#include <iostream>

Game::Game() : timer(120) {
    std::cout << "Initializing game...\n";
    isQuizActive = false;
    isGameOver = false;
    std::cout << "Game initialized.\n";
}

void Game::run() {
    addMessage("Start", "Images/assets/start.png");
    displayMessages();

    if (isGameOver) return;

    while (gameBoard.getWindow().isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::addMessage(const std::string& type, const std::string& assetPath) {
    messages.push_back(GameMessage::createMessage(gameBoard.getWindow(), type, assetPath));
}

void Game::displayMessages() {
    for (auto& message : messages) {
        message->display();
    }
    messages.clear();
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

void Game::handleQuizOptionSelection(sf::Event::MouseButtonEvent mouseButton) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(gameBoardQuiz.getWindowQuiz());
    int clickedOption = gameBoardQuiz.getOptionAtPosition(mousePosition);

    if (clickedOption != -1 && currentQuestion) {
        if (currentQuestion->checkAnswer(clickedOption)) {
            std::cout << "Correct answer!" << std::endl;
            handleCorrectAnswer();
        } else {
            std::cout << "Wrong answer!" << std::endl;
            resetGameAfterWrongAnswer();
        }
    }
}

void Game::handleCorrectAnswer() {
    gameBoardQuiz.getWindowQuiz().close();
    isQuizActive = false;

    if (copiedFirstCard) {
        const std::string& animal = copiedFirstCard->getAnimal();

        if (!animal.empty() &&
            std::find(correctlyAnsweredAnimals.begin(), correctlyAnsweredAnimals.end(), animal) == correctlyAnsweredAnimals.end()) {
            correctlyAnsweredAnimals.push_back(animal);
        }
    }

    copiedFirstCard = nullptr;
}

void Game::resetGameAfterWrongAnswer() {
    gameBoardQuiz.getWindowQuiz().close();
    isQuizActive = false;
    gameBoard.shuffleCards();
    gameBoard.initializeCards();
    firstFlippedCard = nullptr;
    secondFlippedCard = nullptr;
    correctlyAnsweredAnimals.clear();
}

void Game::update() {
    timer.update();

    if (timer.isTimeUp()) {
        if (isQuizActive) {
            gameBoardQuiz.getWindowQuiz().close();
            isQuizActive = false;
        }

        addMessage("Failure", "Images/assets/final.png");
        displayMessages();
        restartGame();
        return;
    }

    if (allQuestionsAnsweredCorrectly()) {
        addMessage("Success", "Images/assets/success.png");
        displayMessages();
        isGameOver = true;
    }

    if (isCheckingMatch && !isQuizActive) {
        if (matchTimer.getElapsedTime().asSeconds() >= matchDelay) {
            handleMatch();
        }
    }
}


void Game::handleMatch() {
    if (firstFlippedCard && secondFlippedCard) {
        if (firstFlippedCard->getAnimal() == secondFlippedCard->getAnimal()) {
            copiedFirstCard = firstFlippedCard->clone();

            auto it = QuestionFactory::animalToQuestionMap.find(firstFlippedCard->getAnimal());
            if (it != QuestionFactory::animalToQuestionMap.end()) {
                currentQuestion = std::move(it->second);
                openQuestionWindow();
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
    if (currentQuestion) {
        gameBoardQuiz.createWindow();
        gameBoardQuiz.setCurrentQuestion(currentQuestion.get());
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
    os << "GameBoard: " << game.gameBoard << "\n";
    return os;
}

bool Game::allQuestionsAnsweredCorrectly() {
    const auto& roundAnimals = gameBoard.getCurrentRoundAnimals();
    for (const auto& animal : roundAnimals) {
        if (std::find(correctlyAnsweredAnimals.begin(), correctlyAnsweredAnimals.end(), animal) == correctlyAnsweredAnimals.end()) {
            return false;
        }
    }
    return true;
}

void Game::restartGame() {
    timer.reset(120);
    isQuizActive = false;
    isGameOver = false;
    firstFlippedCard = nullptr;
    secondFlippedCard = nullptr;
    correctlyAnsweredAnimals.clear();
    gameBoard.initializeCards();
}
