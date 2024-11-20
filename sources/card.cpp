#include "../headers/card.h"

Card::Card(const std::string &animal, sf::Texture& frontTexture, sf::Texture& backTexture):
    animal(animal), matched(false), isFlipped(false)
{
    frontSprite.setTexture(frontTexture);
    backSprite.setTexture(backTexture);

    frontSprite.setScale(0.4f, 0.6f);
    backSprite.setScale(0.4f, 0.6f);
}

Card::Card(const Card& other) :
    animal(other.animal),
    matched(other.matched),
    isFlipped(other.isFlipped)
{
    frontSprite = other.frontSprite;
    backSprite = other.backSprite;
}

Card::~Card() {};

const std::string& Card::getAnimal() const {
    return animal;
}

void Card::draw(sf::RenderWindow& window) const {
    if (isFlipped) {
        window.draw(frontSprite);
    } else {
        window.draw(backSprite);
    }
}

bool Card::isMatched() const {
    return matched;
}

void Card::setMatched(bool is_matched) {
   matched = is_matched;
}

void Card::flip() {
    isFlipped = !isFlipped;
}

void Card::setPosition(float x, float y) {
    frontSprite.setPosition(x, y);
    backSprite.setPosition(x, y);
}


sf::FloatRect Card::getGlobalBounds() const {
    return frontSprite.getGlobalBounds();
}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        animal = other.animal;
        matched = other.matched;
        isFlipped = other.isFlipped;

        frontSprite = other.frontSprite;
        backSprite = other.backSprite;
    }
    return *this;
}

