#ifndef CARD_H
#define CARD_H

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
    ~Card();

    const std::string& getAnimal() const;
    void draw(sf::RenderWindow& window) const;
    bool isMatched() const;
    void setMatched(bool is_matched);
    void flip();
    bool is_flipped() const { return isFlipped; }
    void setPosition(float x, float y);
    sf::FloatRect getGlobalBounds() const;
};

#endif
