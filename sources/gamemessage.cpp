#include "../headers/gamemessage.h"
#include "../headers/gameexception.h"

GameMessage::GameMessage(const std::string& message, sf::RenderWindow& win, const std::string& bgPath)
    : window(win) {
    if (!font.loadFromFile("font/font2.ttf")) {
        throw FileNotFoundException("gamemessage font");
    }

    messageText.setFont(font);
    messageText.setString(message);
    messageText.setCharacterSize(120);

    setBackground(bgPath);

    const sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) / 3.f
    );
}

void GameMessage::setBackground(const std::string& bgPath) {
    if (!backgroundTexture.loadFromFile(bgPath)) {
        throw FileNotFoundException("gamemessage background");
    }
    backgroundSprite.setTexture(backgroundTexture);
}

StartMessage::StartMessage(sf::RenderWindow& win)
    : GameMessage("Memory Game: Two by Two\n", win, "Images/start.png"),
      startClicked(false) {
    messageText.setFillColor(sf::Color::White);
    setupStartText();
}

void StartMessage::setupStartText() {
    buttonText.setFont(font);
    buttonText.setString("Start Game");
    buttonText.setCharacterSize(72);
    buttonText.setFillColor(sf::Color::White);

    const sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        static_cast<float>(window.getSize().x) / 2.f - textBounds.width / 2.f,
        static_cast<float>(window.getSize().y) / 2.f + 200.f - textBounds.height / 2.f + 50.f
    );
}

void StartMessage::display() {
    sf::Event event{};
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::FloatRect textBounds = buttonText.getGlobalBounds();
                if (textBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    startClicked = true;
                    return;
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(messageText);
        window.draw(buttonText);
        window.display();
    }
}

SuccessMessage::SuccessMessage(sf::RenderWindow& win)
    : GameMessage("Noah couldn't have done it without you!", win, "Images/success.png") {
    messageText.setCharacterSize(60);

    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) / 2.f + 100.f
    );
    messageText.setFillColor(sf::Color(0, 255, 255));
}

void SuccessMessage::display() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(messageText);
    window.display();

    sf::Event event{};
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
}

std::unique_ptr<GameMessage> SuccessMessage::clone() const {
    return std::make_unique<SuccessMessage>(*this);
}

FailureMessage::FailureMessage(sf::RenderWindow& win)
    : GameMessage("TIME IS UP!", win, "Images/final.png"), restartClicked(false) {
    messageText.setFillColor(sf::Color::Red);
    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setPosition(
        (static_cast<float>(window.getSize().x) - textBounds.width) / 2.f,
        (static_cast<float>(window.getSize().y) - textBounds.height) / 2.f - 20.f
    );
    setupRestartButton();
}

void FailureMessage::setupRestartButton() {
    restartButton.setSize({300.f, 80.f});
    restartButton.setFillColor(sf::Color(24, 43, 54));
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setOutlineThickness(2.f);
    restartButton.setPosition(static_cast<float>(window.getSize().x) / 2.f - 150.f, static_cast<float>(window.getSize().y) / 2.f + 200.f);

    buttonText.setFont(font);
    buttonText.setString("RESTART GAME");
    buttonText.setCharacterSize(50);
    buttonText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        restartButton.getPosition().x + (restartButton.getSize().x - textBounds.width) / 2.f,
        restartButton.getPosition().y + (restartButton.getSize().y - textBounds.height) / 2.f - 18.f
    );
}

void FailureMessage::display() {
    sf::Event event{};
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (restartButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    restartClicked = true;
                    return;
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(messageText);
        window.draw(restartButton);
        window.draw(buttonText);
        window.display();
    }
}

bool FailureMessage::isRestartClicked() const {
    return restartClicked;
}

std::unique_ptr<GameMessage> FailureMessage::clone() const {
    return std::make_unique<FailureMessage>(*this);
}
