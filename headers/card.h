#ifndef CARD_H
#define CARD_H

#include <memory>
#include <SFML/Graphics.hpp>

class Card {
private:
    std::string animal;
    sf::Sprite frontSprite;
    sf::Sprite backSprite;
    bool matched;
    bool isFlipped;
    bool blocked = false;

    bool isMatched() const;
    void flip();

    void setBlocked(bool state) { blocked = state; }
    bool isBlocked() const { return blocked; }

    friend std::ostream& operator<<(std::ostream& os, const Card& card);

public:
    Card(const std::string &animal, sf::Texture& frontTexture, sf::Texture& backTexture);
    Card(const Card& other);
    Card& operator=(const Card& other);
    virtual ~Card() = default;

    void draw(sf::RenderWindow& window) const;
    void setPosition(float x, float y);
    sf::FloatRect getGlobalBounds() const;
    const std::string& getAnimal() const;

protected:
    virtual std::unique_ptr<Card> clone() const {
        return std::make_unique<Card>(*this);
    }

    friend class Game;
};

#endif
