#include "../headers/gameboard.h"
#include "../headers/gameexception.h"
#include "../headers/question.h"
#include "../headers/game.h"
#include <iostream>

GameBoard::GameBoard() : cardsWindow(sf::VideoMode::getFullscreenModes()[0], "Memory Game: Two by two", sf::Style::Close) {
    if (!backgroundTexture.loadFromFile("Images/assets/background.png")) {
        throw FileNotFoundException("Gameboard background");
    }
    backgroundSprite.setTexture(backgroundTexture);
    float scaleX = static_cast<float>(cardsWindow.getSize().x) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(cardsWindow.getSize().y) / backgroundTexture.getSize().y;
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

GameBoard::~GameBoard() {}

void GameBoard::initializeCards() {
    cards.clear();

    if (!backTexture.loadFromFile("Images/assets/cardback.png")) {
        throw FileNotFoundException("cardback image");
    }

    std::vector<std::string> allAnimals;
    for (const auto& [animal, _] : QuestionFactory::animalToQuestionMap) {
        allAnimals.push_back(animal);
    }

    std::random_shuffle(allAnimals.begin(), allAnimals.end());
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