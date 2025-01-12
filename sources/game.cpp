#include "../headers/game.h"
#include "../headers/gamemessage.h"
#include "../headers/gameexception.h"
#include <iostream>

Game::Game() : timer(120), isGameFinished(false){
    isQuizActive = false;
    isGameOver = false;
}

void Game::run() {
    addMessage(MessageType::Start, "Images/assets/start.png");
    displayMessages();

    if (isGameOver) return;

    while (gameBoard.getWindow().isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::addMessage(MessageType type, const std::string& assetPath) {
    messages.push_back(MessageFactory::createMessage(gameBoard.getWindow(), type, assetPath));
}

void Game::displayMessages() {
    if (timer.isClockSoundPlaying()) {
        timer.stopClockSound();
    }
    for (auto& message : messages) {
        message->display();
    }
    messages.clear();
}

void Game::processEvents() {
    if (isGameFinished) {
        return;
    }
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

        if (!clickedCard) {
            return;
        }

        if (clickedCard->isMatched()) {
            return;
        }

        if (clickedCard == firstFlippedCard) {
            return;
        }

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
    if (!currentQuestion) {
        throw GameLogicException("No active question available for the quiz.");
    }

    sf::Vector2i mousePosition = sf::Mouse::getPosition(gameBoardQuiz.getWindowQuiz());
    int clickedOption = gameBoardQuiz.getOptionAtPosition(mousePosition);

    if (clickedOption != -1) {
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
    gameBoard.positionCards();
    gameBoard.initializeCards();

    firstFlippedCard = nullptr;
    secondFlippedCard = nullptr;
    correctlyAnsweredAnimals.clear();

    QuestionFactory::loadQuestionsFromFile("Init/questions.txt", "Init/truefalse.txt");
}

void Game::update() {
    timer.update();

    if (timer.isTimeUp()) {
        if (isQuizActive) {
            gameBoardQuiz.getWindowQuiz().close();
            isQuizActive = false;
        }

        addMessage(MessageType::Failure, "Images/assets/final.png");
        displayMessages();
        restartGame();
        return;
    }

    if (allQuestionsAnsweredCorrectly()) {
        addMessage(MessageType::Success, "Images/assets/success.png");
        displayMessages();
        isGameOver = true;
        isGameFinished = true;
    }

    if (isCheckingMatch && !isQuizActive) {
        if (!firstFlippedCard || !secondFlippedCard) {
            throw GameLogicException("Match checking state is active, but flipped cards are not set.");
        }

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
            } else {
                throw GameLogicException("No question available for the matched animal: " + firstFlippedCard->getAnimal());
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
    if (!currentQuestion) {
        throw GameLogicException("Cannot open question window: no current question set.");
    }

    gameBoardQuiz.createWindow();
    gameBoardQuiz.setCurrentQuestion(currentQuestion.get());
    isQuizActive = true;
}

void Game::render() {
    if (isGameFinished) {
        return;
    }
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
