#ifndef TIMER_H
#define TIMER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class Timer {
private:
    int timeRemaining;
    sf::Clock clock;
    sf::Text timeText;
    sf::Font font;

    sf::SoundBuffer clockSoundBuffer;
    sf::Sound clockSound;

public:
    explicit Timer(int startTime);
    void updateText();
    void update();
    void render(sf::RenderWindow& window) const;
    bool isTimeUp() const;
    void reset(int initialTime);
    bool isClockSoundPlaying() const;
    void stopClockSound();
};
#endif // TIMER_H
