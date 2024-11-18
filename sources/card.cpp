#include "../headers/card.h"

Card::Card(std::string animal, sf::Texture& frontTexture, sf::Texture& backTexture):
    animal(animal), matched(false), isFlipped(false)
{
    frontSprite.setTexture(frontTexture);
    backSprite.setTexture(backTexture);

    frontSprite.setScale(0.4f, 0.6f);
    backSprite.setScale(0.4f, 0.6f);
}

std::string Card::getAnimal() const {
    return animal;
}

void Card::draw(sf::RenderWindow& window) {
    if (isFlipped) {
        window.draw(frontSprite);
    } else {
        window.draw(backSprite);
    }
}

bool Card::isMatched() const {
    return matched;
}

void Card::setMatched(bool matched) {
    this->matched = matched;
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
