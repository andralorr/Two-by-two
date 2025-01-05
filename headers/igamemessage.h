#ifndef IGAMEMESSAGE_H
#define IGAMEMESSAGE_H

#pragma once
#include <SFML/Graphics.hpp>

class IGameMessage {
public:
    virtual ~IGameMessage() = default;

    virtual void display(sf::RenderWindow& window) = 0;
};

#endif //IGAMEMESSAGE_H
