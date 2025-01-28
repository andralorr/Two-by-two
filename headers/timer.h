#ifndef TIMER_H
#define TIMER_H

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/singleton.h"

class Timer : public Singleton<Timer, false> {
    friend class Singleton<Timer, false>;

private:
    Timer(int startTime);
    int timeRemaining;
    sf::Font font;
    sf::Text timeText;
    sf::Clock clock;
    sf::SoundBuffer clockSoundBuffer;
    sf::Sound clockSound;

    void updateText();
public:
    ~Timer();
    void render(sf::RenderWindow& window) const;
    void update();
    void reset(int initialTime);
    bool isClockSoundPlaying() const;
    void stopClockSound();
    bool isTimeUp() const;
};

#endif //TIMER_H