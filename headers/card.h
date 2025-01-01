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

public:
    Card(const std::string &animal, sf::Texture& frontTexture, sf::Texture& backTexture);
    Card(const Card& other);
    Card& operator=(const Card& other);

    const std::string& getAnimal() const;
    void draw(sf::RenderWindow& window) const;
    bool isMatched() const;
    void setMatched(bool is_matched);
    void flip();
    void setPosition(float x, float y);
    sf::FloatRect getGlobalBounds() const;
    friend std::ostream& operator<<(std::ostream& os, const Card& card);
    virtual ~Card() = default;
protected:
    virtual std::unique_ptr<Card> clone() const {
        return std::make_unique<Card>(*this);
    }

    friend class Game;
};

#endif
