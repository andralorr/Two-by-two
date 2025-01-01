#include "../headers/game.h"
#include "../headers/question.h"
#include "../headers/gameexception.h"
#include "../headers/gamemessage.h"
#include <iostream>
#include <memory>

Game::Game(): timer(120) {
    std::cout << "Initializing game...\n";
    isQuizActive = false;
    currentQuestion = nullptr;
    isGameOver = false;
    std::cout << "Game initialized.\n";
}

void Game::run() {
    addMessage(std::make_unique<StartMessage>(gameBoard.getWindow(), "Images/assets/start.png"));
    displayMessages();

    if (isGameOver) return;

    while (gameBoard.getWindow().isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::addMessage(std::unique_ptr<GameMessage> message) {
    messages.push_back(std::move(message));
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

        if (clickedCard && !clickedCard->isMatched() &&  clickedCard != firstFlippedCard) {
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
            if (const auto* threeOptions = dynamic_cast<ThreeOptionsQuestion*>(currentQuestion)) {
                if (threeOptions->checkAnswer(clickedOption)) {
                    std::cout << "Correct answer!" << std::endl;
                    answeredQuestions.push_back(currentQuestion);
                    handleCorrectAnswer();
                } else {
                    std::cout << "Wrong answer!" << std::endl;
                    resetGameAfterWrongAnswer();
                }
            } else if (const auto* trueFalse = dynamic_cast<TrueFalseQuestion*>(currentQuestion)) {
                if (trueFalse->checkAnswer(clickedOption)) {
                    std::cout << "Correct answer!" << std::endl;
                    answeredQuestions.push_back(currentQuestion);
                    handleCorrectAnswer();
                } else {
                    std::cout << "Wrong answer!" << std::endl;
                    resetGameAfterWrongAnswer();
                }
            }
        } else {
            throw InvalidStateException("No question available for the selected card");
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
            std::cout << "Added to correctlyAnsweredAnimals: " << animal << std::endl;
            } else {
                std::cout << "Animal already in correctlyAnsweredAnimals or empty: " << animal << std::endl;
            }
    } else {
        std::cerr << "Error: Copied card is null." << std::endl;
    }

    copiedFirstCard = nullptr;
    currentQuestion = nullptr;
}


void Game::resetGameAfterWrongAnswer() {
    gameBoardQuiz.getWindowQuiz().close();
    isQuizActive = false;
    currentQuestion = nullptr;
    gameBoard.shuffleCards();

    QuestionFactory::loadQuestionsFromFile("Init/questions.txt", "Init/truefalse.txt");
    gameBoard.getCards().clear();
    gameBoard.initializeCards();

    firstFlippedCard = nullptr;
    secondFlippedCard = nullptr;
    isCheckingMatch = false;
    correctlyAnsweredAnimals.clear();
}


void Game::update() {
    timer.update();

    if (timer.isTimeUp()) {
        addMessage(std::make_unique<FailureMessage>(gameBoard.getWindow(), "Images/assets/final.png"));
        displayMessages();

        if (isGameOver) return;
        restartGame();
        return;
    }

    if (allQuestionsAnsweredCorrectly()) {
        std::cout << "All questions answered correctly! Displaying success message." << std::endl;
        addMessage(std::make_unique<SuccessMessage>(gameBoard.getWindow(), "Images/assets/success.png"));
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
        std::cout << "Checking cards:" << std::endl;
        std::cout << *firstFlippedCard << std::endl;
        std::cout << *secondFlippedCard << std::endl;
        if (firstFlippedCard->getAnimal() == secondFlippedCard->getAnimal()) {
            copiedFirstCard = firstFlippedCard->clone();
            auto it = QuestionFactory::animalToQuestionMap.find(firstFlippedCard->getAnimal());
            if (it != QuestionFactory::animalToQuestionMap.end()) {
                currentQuestion = it->second.get();
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
    const std::vector<std::string>& roundAnimals = gameBoard.getCurrentRoundAnimals();

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
    currentQuestion = nullptr;
    answeredQuestions.clear();
    correctlyAnsweredAnimals.clear();

    gameBoard.getCards().clear();
    gameBoard.initializeCards();

    std::cout << "\n\nGame has been restarted!\n\n" << std::endl;
}
