#ifndef TIMER_H
#define TIMER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Timer {
private:
    int timeRemaining;
    sf::Clock clock;
    sf::Text timeText;
    sf::Font font;

public:
    Timer(int startTime);
    void updateText();
    void update();
    void render(sf::RenderWindow& window) const;
    bool isTimeUp() const;
    void reset(int initialTime);
};
#endif // TIMER_H
