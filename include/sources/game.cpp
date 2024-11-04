#include "../headers/game.h"
#include <random>
#include <iostream>

Game::Game() : window(sf::VideoMode(800, 600), "Memory Game: Two by two"),
               timeLimit(sf::seconds(300)), firstFlippedCard(nullptr) {
    if (!backgroundTexture.loadFromFile("../include/images/background.png")) {
        std::cerr << "Error: background texture " << std::endl;
        exit(1);
    }
    backgroundSprite.setTexture(backgroundTexture);
    initializeCards();
    positionCards();
    shuffleCards();
}
void Game::initializeCards() {
    std::vector<std::string> animalNames = {"cat", "chameleon", "deer", "koala", "monkey", "parrot"};

    if (!backTexture.loadFromFile("../include/images/cardback.png")) {
        std::cerr << "Error: backcard texture" << std::endl;
        exit(1);
    }

    for (const auto& animal : animalNames) {
        auto frontTexture = std::make_shared<sf::Texture>();
        if (!frontTexture->loadFromFile("../include/images/" + animal + ".png")) {
            std::cerr << "Error: front texture " << animal << ".png" << std::endl;
            exit(1);
        }
        frontTextures.push_back(frontTexture);

        cards.emplace_back(animal, *frontTextures.back(), backTexture);
        cards.emplace_back(animal, *frontTextures.back(), backTexture);
    }
    shuffleCards();
}
void Game::positionCards() {
    const int rows = 3;
    const int cols = 4;
    const int cardWidth = 80;
    const int cardHeight = 160;
    const int spacing = 30;

    int startX = (window.getSize().x - (cols * cardWidth + (cols - 1) * spacing)) / 2;
    int startY = (window.getSize().y - (rows * cardHeight + (rows - 1) * spacing)) / 2;

    int index = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = startX + col * (cardWidth + spacing);
            int y = startY + row * (cardHeight + spacing);
            cards[index].setPosition(x, y);
            ++index;
        }
    }
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                Card* clickedCard = getCardAtPosition(mousePosition);

                if (clickedCard && !clickedCard->isMatched() && clickedCard != firstFlippedCard) {
                    clickedCard->flip();

                    if (!firstFlippedCard) {
                        firstFlippedCard = clickedCard;
                    } else {
                        secondFlippedCard = clickedCard;
                        isCheckingMatch = true;
                        flipClock.restart();
                    }
                }
            }
        }
    }
}

void Game::update() {
    if (isCheckingMatch && flipClock.getElapsedTime().asSeconds() > 0.5f) {
        if (firstFlippedCard->getAnimal() == secondFlippedCard->getAnimal()) {
            firstFlippedCard->setMatched(true);
            secondFlippedCard->setMatched(true);
        } else {
            firstFlippedCard->flip();
            secondFlippedCard->flip();
        }
        firstFlippedCard = nullptr;
        secondFlippedCard = nullptr;
        isCheckingMatch = false;
    }
}


void Game::render() {
    window.clear();
    window.draw(backgroundSprite);
    for (auto& card : cards) {
        card.draw(window);
    }
    window.display();
}

void Game::shuffleCards() {
    //...
}

Card* Game::getCardAtPosition(sf::Vector2i position) {
    for (auto& card : cards) {
        if (card.getGlobalBounds().contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
            return &card;
        }
    }
    return nullptr;
}
