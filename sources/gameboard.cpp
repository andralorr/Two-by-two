#include "../headers/gameboard.h"
#include "../headers/gameexception.h"
#include "../headers/question.h"
#include "../headers/game.h"
#include <iostream>
#include <random>

GameBoard::GameBoard() : cardsWindow(sf::VideoMode::getDesktopMode(), "Memory Game: Two by two", sf::Style::Close) {
    if (!backgroundTexture.loadFromFile("Images/assets/background.png")) {
        throw FileNotFoundException("Gameboard background");
    }
    backgroundSprite.setTexture(backgroundTexture);
    float scaleX = static_cast<float>(cardsWindow.getSize().x) / static_cast<float>(backgroundTexture.getSize().x);
    float scaleY = static_cast<float>(cardsWindow.getSize().y) / static_cast<float>(backgroundTexture.getSize().y);

    backgroundSprite.setScale(scaleX, scaleY);

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

GameBoard::~GameBoard() = default;

void GameBoard::initializeCards() {
    cards.clear();

    if (!backTexture.loadFromFile("Images/assets/cardback.png")) {
        throw FileNotFoundException("cardback image");
    }

    std::vector<std::string> allAnimals;
    for (const auto& [animal, _] : QuestionFactory::animalToQuestionMap) {
        allAnimals.push_back(animal);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allAnimals.begin(), allAnimals.end(), g);
    currentRoundAnimals.assign(allAnimals.begin(), allAnimals.begin() + 6);

    for (const auto& animal : currentRoundAnimals) {
        sf::Texture frontTexture;
        if (!frontTexture.loadFromFile("Images/animals/" + animal + ".png")) {
            throw FileNotFoundException("Image for animal: " + animal);
        }
        frontTextures.push_back(std::make_shared<sf::Texture>(frontTexture));

        cards.emplace_back(animal, *frontTextures.back(), backTexture);
        cards.emplace_back(animal, *frontTextures.back(), backTexture);
    }
    shuffleCards();
}

void GameBoard::positionCards() {
    const int rows = 3;
    const int cols = 4;
    const int cardWidth = 120;
    const int cardHeight = 200;
    const int spacing = 30;

    const int totalWidth = cols * cardWidth + (cols - 1) * spacing;
    const int totalHeight = rows * cardHeight + (rows - 1) * spacing;

    const int startX = (static_cast<int>(cardsWindow.getSize().x) - totalWidth) / 2;
    const int startY = (static_cast<int>(cardsWindow.getSize().y) - totalHeight) / 2;

    int index = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            const int x = startX + col * (cardWidth + spacing);
            const int y = startY + row * (cardHeight + spacing);
            cards[index].setPosition(static_cast<float>(x), static_cast<float>(y));
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
}

void GameBoard::shuffleCards() {
    std::srand(unsigned(std::time(0)));
    for (int i = cards.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(cards[i], cards[j]);
    }
    positionCards();
}

Card* GameBoard::getCardAtPosition(const sf::Vector2i& position) {
    for (auto& card : cards) {
        sf::FloatRect bounds = card.getGlobalBounds();
        if (bounds.contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
            return &card;
        }
    }
    return nullptr;
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