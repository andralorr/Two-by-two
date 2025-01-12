#include "../headers/gamemessage.h"
#include <stdexcept>

GameMessage::GameMessage(sf::RenderWindow& win, const std::string& bgPath)
    : window(win) {
    if (!font.loadFromFile("font/font2.ttf")) {
        throw std::runtime_error("Failed to load font: font/font2.ttf");
    }
    if (!backgroundTexture.loadFromFile(bgPath)) {
        throw std::runtime_error("Failed to load background texture: " + bgPath);
    }
    backgroundSprite.setTexture(backgroundTexture);

    float scaleX = static_cast<float>(window.getSize().x) / static_cast<float>(backgroundTexture.getSize().x);
    float scaleY = static_cast<float>(window.getSize().y) / static_cast<float>(backgroundTexture.getSize().y);

    backgroundSprite.setScale(scaleX, scaleY);
    window.setVerticalSyncEnabled(true);
}

void GameMessage::display() {
    sf::Event event{};
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                if (isButtonClicked(mousePosition)) {
                    return;
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(messageText);
        customizeDisplay();
        window.display();
    }
}

bool GameMessage::isButtonClicked(const sf::Vector2f&) const {
    return false;
}

StartMessage::StartMessage(sf::RenderWindow& win, const std::string& bgPath)
    : GameMessage(win, bgPath) {
    messageText.setFont(font);
    messageText.setString("Memory Game: Two by Two");
    messageText.setCharacterSize(120);
    messageText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = messageText.getLocalBounds();

    float titleX = (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f;
    float titleY = static_cast<float>(window.getSize().y) * 0.29f;
    messageText.setPosition(titleX, titleY);

    buttonText.setFont(font);
    buttonText.setString("START GAME");
    buttonText.setCharacterSize(80);
    buttonText.setFillColor(sf::Color::White);

    textBounds = buttonText.getLocalBounds();

    float buttonX = (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f;
    float buttonY = static_cast<float>(window.getSize().y) * 0.72f;
    buttonText.setPosition(buttonX, buttonY);
}

void StartMessage::customizeDisplay() {
    window.draw(buttonText);
}

bool StartMessage::isButtonClicked(const sf::Vector2f& mousePosition) const {
    sf::FloatRect bounds = buttonText.getGlobalBounds();
    return bounds.contains(mousePosition);
}

SuccessMessage::SuccessMessage(sf::RenderWindow& win, const std::string& bgPath)
    : GameMessage(win, bgPath) {
    messageText.setFont(font);
    messageText.setString("Noah couldn't have done it without you!");
    messageText.setCharacterSize(80);
    messageText.setFillColor(sf::Color(0, 255, 255));

    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) * 0.65f
    );
}

void SuccessMessage::customizeDisplay() {}

FailureMessage::FailureMessage(sf::RenderWindow& win, const std::string& bgPath)
    : GameMessage(win, bgPath) {
    messageText.setFont(font);
    messageText.setString("TIME IS UP!");
    messageText.setCharacterSize(92);
    messageText.setFillColor(sf::Color::Red);

    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (win.getSize().x - textBounds.width) / 2.f,
        (win.getSize().y * 0.45f)
    );


    buttonShape.setSize({300.0f, 80.0f});
    buttonShape.setFillColor(sf::Color(24, 43, 54));
    buttonShape.setOutlineColor(sf::Color::White);
    buttonShape.setOutlineThickness(2.f);

    buttonShape.setPosition(
        (win.getSize().x - buttonShape.getSize().x) / 2.f,
        win.getSize().y * 0.7f
    );

    buttonText.setFont(font);
    buttonText.setString("RESTART GAME");
    buttonText.setCharacterSize(50);
    buttonText.setFillColor(sf::Color::White);

    textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        buttonShape.getPosition().x + (buttonShape.getSize().x - textBounds.width) / 2.f,
        buttonShape.getPosition().y + (buttonShape.getSize().y - textBounds.height) * 0.2f
    );
}

void FailureMessage::customizeDisplay() {
    window.draw(buttonShape);
    window.draw(buttonText);
}

bool FailureMessage::isButtonClicked(const sf::Vector2f& mousePosition) const {
    sf::FloatRect bounds = buttonShape.getGlobalBounds();
    return bounds.contains(mousePosition);
}

std::unique_ptr<IGameMessage> MessageFactory::createMessage(sf::RenderWindow& win, MessageType type, const std::string& bgPath) {
    switch (type) {
        case MessageType::Start:
            return std::make_unique<StartMessage>(win, bgPath);
        case MessageType::Success:
            return std::make_unique<SuccessMessage>(win, bgPath);
        case MessageType::Failure:
            return std::make_unique<FailureMessage>(win, bgPath);
        default:
            throw std::invalid_argument("unknown MessageType");
    }
}
