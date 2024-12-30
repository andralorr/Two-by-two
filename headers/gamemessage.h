#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class GameMessage {
protected:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text messageText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    float titleOffsetY = 0.0f;
    virtual void customizeDisplay();

public:
    explicit GameMessage(sf::RenderWindow& win, const std::string& bgPath);
    virtual ~GameMessage() = default;

    virtual void display();
};

class StartMessage : public GameMessage {
    sf::Text buttonText;

public:
    StartMessage(sf::RenderWindow& win, const std::string& bgPath);
    void customizeDisplay() override;
};

class SuccessMessage : public GameMessage {
public:
    SuccessMessage(sf::RenderWindow& win, const std::string& bgPath);
    void customizeDisplay() override;
};

class FailureMessage : public GameMessage {
    sf::Text buttonText;
    sf::RectangleShape buttonShape;

public:
    FailureMessage(sf::RenderWindow& win, const std::string& bgPath);
    void customizeDisplay() override;
};
