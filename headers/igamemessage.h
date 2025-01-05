#ifndef IGAMEMESSAGE_H
#define IGAMEMESSAGE_H

#pragma once

class IGameMessage {
public:
    virtual ~IGameMessage() = default;

    virtual void display() = 0;
};

#endif //IGAMEMESSAGE_H
