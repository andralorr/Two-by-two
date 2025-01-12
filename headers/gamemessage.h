#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "igamemessage.h"

class GameMessage : public IGameMessage {
protected:
    sf::RenderWindow& window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text messageText;

public:
    explicit GameMessage(sf::RenderWindow& win, const std::string& bgPath);
    ~GameMessage() override = default;

    void display() override;
    virtual void customizeDisplay() = 0;
    virtual bool isButtonClicked(const sf::Vector2f& mousePosition) const;
};

class StartMessage : public GameMessage {
    sf::Text buttonText;
    float titleOffsetY;

public:
    explicit StartMessage(sf::RenderWindow& win, const std::string& bgPath);
    void customizeDisplay() override;
    bool isButtonClicked(const sf::Vector2f& mousePosition) const override;
};

class SuccessMessage : public GameMessage {
public:
    explicit SuccessMessage(sf::RenderWindow& win, const std::string& bgPath);
    void customizeDisplay() override;
};

class FailureMessage : public GameMessage {
    sf::RectangleShape buttonShape;
    sf::Text buttonText;

public:
    explicit FailureMessage(sf::RenderWindow& win, const std::string& bgPath);
    void customizeDisplay() override;
    bool isButtonClicked(const sf::Vector2f& mousePosition) const override;
};

enum class MessageType {
    Start,
    Success,
    Failure
};

class MessageFactory {
public:
    static std::unique_ptr<IGameMessage> createMessage(sf::RenderWindow& win, MessageType type, const std::string& bgPath);
};
