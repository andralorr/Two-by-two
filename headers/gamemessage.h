#ifndef GAMEMESSAGE_H
#define GAMEMESSAGE_H

#include <SFML/Graphics.hpp>
#include <string>

class GameMessage {
protected:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text messageText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    virtual ~GameMessage() = default;

    GameMessage(const std::string& message, sf::RenderWindow& win, const std::string& bgPath);
    virtual void display() = 0;

    void setBackground(const std::string& bgPath);

};

class StartMessage : public GameMessage {
private:
    sf::RectangleShape startButton;
    sf::Text buttonText;
    bool startClicked;

    void setupStartText();

public:
    explicit StartMessage(sf::RenderWindow& win);
    void display() override;
};

class SuccessMessage : public GameMessage {
public:
    explicit SuccessMessage(sf::RenderWindow& win);
    void display() override;
};

class FailureMessage : public GameMessage {
private:
    sf::RectangleShape restartButton;
    sf::Text buttonText;
    bool restartClicked;

    void setupRestartButton();

public:
    explicit FailureMessage(sf::RenderWindow& win);
    void display() override;
    bool isRestartClicked() const;
};

#endif
