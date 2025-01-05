#include "../headers/gamemessage.h"
#include <stdexcept>

// Base GameMessage implementation
GameMessage::GameMessage(sf::RenderWindow& win, const std::string& bgPath)
    : window(win) {
    if (!font.loadFromFile("font/font2.ttf")) {
        throw std::runtime_error("Failed to load font: font/font2.ttf");
    }
    if (!backgroundTexture.loadFromFile(bgPath)) {
        throw std::runtime_error("Failed to load background texture: " + bgPath);
    }
    backgroundSprite.setTexture(backgroundTexture);
}

void GameMessage::display() {
    sf::Event event{};
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                return;
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(messageText);
        customizeDisplay();
        window.display();
    }
}

// Factory method for creating messages
std::unique_ptr<IGameMessage> GameMessage::createMessage(sf::RenderWindow& win, const std::string& type, const std::string& bgPath) {
    if (type == "Start") {
        return std::make_unique<StartMessage>(win, bgPath);
    } else if (type == "Success") {
        return std::make_unique<SuccessMessage>(win, bgPath);
    } else if (type == "Failure") {
        return std::make_unique<FailureMessage>(win, bgPath);
    } else {
        throw std::invalid_argument("Unknown message type: " + type);
    }
}

// StartMessage implementation
StartMessage::StartMessage(sf::RenderWindow& win, const std::string& bgPath)
    : GameMessage(win, bgPath) {
    messageText.setFont(font);
    messageText.setString("Memory Game: Two by Two");
    messageText.setCharacterSize(120);
    messageText.setFillColor(sf::Color::White);
    titleOffsetY = -30.0f;

    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) / 3.f + titleOffsetY
    );

    buttonText.setFont(font);
    buttonText.setString("START GAME");
    buttonText.setCharacterSize(68);
    buttonText.setFillColor(sf::Color::White);

    textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) / 2.f + 240.f)
    );
}

void StartMessage::customizeDisplay() {
    window.draw(buttonText);
}

// SuccessMessage implementation
SuccessMessage::SuccessMessage(sf::RenderWindow& win, const std::string& bgPath)
    : GameMessage(win, bgPath) {
    messageText.setFont(font);
    messageText.setString("Noah couldn't have done it without you!");
    messageText.setCharacterSize(60);
    messageText.setFillColor(sf::Color(0, 255, 255));

    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) / 2.f + 90.0f
    );
}

void SuccessMessage::customizeDisplay() {}

// FailureMessage implementation
FailureMessage::FailureMessage(sf::RenderWindow& win, const std::string& bgPath)
    : GameMessage(win, bgPath) {
    messageText.setFont(font);
    messageText.setString("TIME IS UP!");
    messageText.setCharacterSize(92);
    messageText.setFillColor(sf::Color::Red);

    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) / 3.f + 160.f
    );

    buttonShape.setSize({300.f, 80.f});
    buttonShape.setFillColor(sf::Color(24, 43, 54));
    buttonShape.setOutlineColor(sf::Color::White);
    buttonShape.setOutlineThickness(2.f);

    buttonShape.setPosition(
        (static_cast<float>(window.getSize().x) - buttonShape.getSize().x) / 2.f,
        (static_cast<float>(window.getSize().y) / 2.f + 200.f)
    );

    buttonText.setFont(font);
    buttonText.setString("RESTART GAME");
    buttonText.setCharacterSize(50);
    buttonText.setFillColor(sf::Color::White);

    textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        buttonShape.getPosition().x + (buttonShape.getSize().x - textBounds.width) / 2.f,
        buttonShape.getPosition().y + (buttonShape.getSize().y - textBounds.height) / 2.f - 16.f
    );
}

void FailureMessage::customizeDisplay() {
    window.draw(buttonShape);
    window.draw(buttonText);
}
