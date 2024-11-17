#include "game.h"

void Game::run() {
    while (gameBoard.getWindow().isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
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
                    }
                }
            }
        }
    }
}

void Game::update() {
    if (isCheckingMatch) {
        handleMatch();
    }
}

void Game::handleMatch() {
    if (firstFlippedCard->getAnimal() == secondFlippedCard->getAnimal()) {

        openQuestionWindow();
    } else {

        firstFlippedCard->flip();
        secondFlippedCard->flip();
    }

    firstFlippedCard = nullptr;
    secondFlippedCard = nullptr;
    isCheckingMatch = false;
}

void Game::openQuestionWindow() {
    questionWindow.create(sf::VideoMode(400, 300), "Intrebare!");

    while (questionWindow.isOpen()) {
        sf::Event event;
        while (questionWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                questionWindow.close();
            }
        }

        questionWindow.clear();
        questionWindow.display();
    }
}

void Game::render() {
    gameBoard.render();
}
