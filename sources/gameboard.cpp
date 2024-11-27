#include "../headers/gameboard.h"
#include <iostream>

GameBoard::GameBoard() : cardsWindow(sf::VideoMode::getFullscreenModes()[0], "Memory Game: Two by two", sf::Style::Close) {
    if (!backgroundTexture.loadFromFile("Images/background.png")) {
        std::cerr << "Error: background texture " << std::endl;
        exit(1);
    }
    backgroundSprite.setTexture(backgroundTexture);
    float scaleX = static_cast<float>(cardsWindow.getSize().x) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(cardsWindow.getSize().y) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    cardsWindow.setFramerateLimit(60);
    cardsWindow.setVerticalSyncEnabled(true);

    initializeCards();
    positionCards();
    shuffleCards();
}

GameBoard::GameBoard(const GameBoard& other) :
    backgroundTexture(other.backgroundTexture),
    backgroundSprite(other.backgroundSprite),
    backTexture(other.backTexture),
    frontTextures(other.frontTextures),
    cards(other.cards)
{
}

GameBoard::~GameBoard() {}

void GameBoard::initializeCards() {
    std::vector<std::string> animalNames = {"giraffe", "chameleon", "deer", "owl", "monkey", "parrot"};

    if (!backTexture.loadFromFile("Images/cardback.png")) {
        std::cerr << "Error: backcard texture" << std::endl;
        exit(1);
    }

    for (const auto& animalName : animalNames) {
        sf::Texture frontTexture;
        if (!frontTexture.loadFromFile("Images/" + animalName + ".png")) {
            std::cerr << "Error: front texture " << animalName << std::endl;
            exit(1);
        }
        frontTextures.push_back(std::make_shared<sf::Texture>(frontTexture));

        cards.emplace_back(animalName, *frontTextures.back(), backTexture);
        cards.emplace_back(animalName, *frontTextures.back(), backTexture);
    }
    shuffleCards();
}

void GameBoard::positionCards() {
    const int rows = 3;
    const int cols = 4;
    const int cardWidth = 120;
    const int cardHeight = 200;
    const int spacing = 30;

    int startX = (cardsWindow.getSize().x - (cols * cardWidth + (cols - 1) * spacing)) / 2;
    int startY = (cardsWindow.getSize().y - (rows * cardHeight + (rows - 1) * spacing)) / 2;

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

void GameBoard::render() {
    cardsWindow.clear();
    cardsWindow.draw(backgroundSprite);
    for (const auto& card : cards) {
        card.draw(cardsWindow);
    }
    cardsWindow.display();
}

void GameBoard::shuffleCards() {
    std::srand(unsigned(std::time(0)));
    for (int i = cards.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(cards[i], cards[j]);
    }
    positionCards();
}

Card* GameBoard::getCardAtPosition(sf::Vector2i position) {
    for (auto& card : cards) {
        if (card.getGlobalBounds().contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
            return &card;
        }
    }
    return nullptr;
}
std::vector<Card>& GameBoard::getCards() {
    return cards;
}

sf::RenderWindow& GameBoard::getWindow() {
    return cardsWindow;
}

GameBoard& GameBoard::operator=(const GameBoard& other) {
    if (this != &other) {
        backgroundTexture = other.backgroundTexture;
        backgroundSprite = other.backgroundSprite;
        backTexture = other.backTexture;
        frontTextures = other.frontTextures;
        cards = other.cards;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const GameBoard& gameBoard) {
    os << "{ "
    << "Window Size: "<<gameBoard.cardsWindow.getSize().x << "x" << gameBoard.cardsWindow.getSize().y
    << ", Number of Cards: " << gameBoard.cards.size()
    << ", Cards: [ ";
    for (const auto& card : gameBoard.cards) {
        os << card.getAnimal() << " ";
    }
    os << "]}";
    return os;
}
