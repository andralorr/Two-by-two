#include "../headers/gameboard.h"
#include <iostream>

GameBoard::GameBoard() : window(sf::VideoMode(800, 600), "Memory Game: Two by two") {
    if (!backgroundTexture.loadFromFile("../Images/background.png")) {
        std::cerr << "Error: background texture " << std::endl;
        exit(1);
    }
    backgroundSprite.setTexture(backgroundTexture);
    initializeCards();
    positionCards();
    shuffleCards();
}

void GameBoard::initializeCards() {
    std::vector<std::string> animalNames = {"cat", "chameleon", "deer", "koala", "monkey", "parrot"};

    if (!backTexture.loadFromFile("../Images/cardback.png")) {
        std::cerr << "Error: backcard texture" << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < animalNames.size(); ++i) {
        sf::Texture frontTexture;
        if (!frontTexture.loadFromFile("../Images/" + animalNames[i] + ".png")) {
            std::cerr << "Error: front texture " << animalNames[i] << std::endl;
            exit(1);
        }
        frontTextures.push_back(std::make_shared<sf::Texture>(frontTexture));

        cards.emplace_back(animalNames[i], *frontTextures.back(), backTexture, i);
        cards.emplace_back(animalNames[i], *frontTextures.back(), backTexture, i);
    }
    shuffleCards();
}

void GameBoard::positionCards() {
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

void GameBoard::render() {
    window.clear();
    window.draw(backgroundSprite);
    for (auto& card : cards) {
        card.draw(window);
    }
    window.display();
}

void GameBoard::shuffleCards() {
    std::srand(unsigned(std::time(0)));
    for (int i = cards.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(cards[i], cards[j]);
    }
}

Card* GameBoard::getCardAtPosition(sf::Vector2i position) {
    for (auto& card : cards) {
        if (card.getGlobalBounds().contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
            return &card;
        }
    }
    return nullptr;
}

sf::RenderWindow& GameBoard::getWindow() {
    return window;
}

void GameBoard::setupQuestion(const Question& question) {
    currentQuestion = &const_cast<Question&>(question);

    questionText.setString(question.getQuestionText());
    questionText.setCharacterSize(30);
    questionText.setFillColor(sf::Color::White);

    sf::FloatRect questionBounds = questionText.getLocalBounds();
    questionText.setOrigin(questionBounds.width / 2, questionBounds.height / 2);
    questionText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 4.0f);

    optionTexts.clear();
    optionBoxes.clear();
    float startY = window.getSize().y / 2.0f;

    const auto& options = question.getOptions();
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text optionText;

        optionText.setString(options[i]);
        optionText.setCharacterSize(24);
        optionText.setFillColor(sf::Color::Black);

        sf::FloatRect textBounds = optionText.getLocalBounds();
        optionText.setOrigin(textBounds.width / 2, textBounds.height / 2);

        float centerX = window.getSize().x / 2.0f;
        float currentY = startY + i * 50;

        optionText.setPosition(centerX, currentY);

        sf::RectangleShape optionBox;
        optionBox.setSize(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
        optionBox.setFillColor(sf::Color::White);
        optionBox.setOutlineColor(sf::Color::Black);
        optionBox.setOutlineThickness(2);

        optionBox.setOrigin(optionBox.getSize().x / 2, optionBox.getSize().y / 2);
        optionBox.setPosition(centerX, currentY);

        optionTexts.push_back(optionText);
        optionBoxes.push_back(optionBox);
    }
}


void GameBoard::drawQuestion() {
    if (!currentQuestion) {
        return;
    }

    window.draw(questionText);

    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        window.draw(optionBoxes[i]);
        window.draw(optionTexts[i]);
    }
}

const std::vector<sf::RectangleShape>& GameBoard::getOptionBoxes() const {
    return optionBoxes;
}
