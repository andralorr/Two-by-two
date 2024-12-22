#include "../headers/timer.h"
#include <iostream>
#include <stdexcept>

constexpr float TIMER_X_POS = 10.f;
constexpr float TIMER_Y_POS = 10.f;

Timer::Timer(int startTime) {
    timeRemaining = startTime;

    if (!font.loadFromFile("font/font.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    timeText.setFont(font);
    timeText.setCharacterSize(28);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(TIMER_X_POS, TIMER_Y_POS);

    clock.restart();
}

void Timer::update() {
    if (timeRemaining > 0 && clock.getElapsedTime().asSeconds() >= 1) {
        timeRemaining--;
        clock.restart();
    }

    if (timeRemaining <= 30 && timeText.getFillColor() != sf::Color::Red) {
        timeText.setFillColor(sf::Color::Red);
    } else if (timeRemaining > 30 && timeText.getFillColor() != sf::Color::White) {
        timeText.setFillColor(sf::Color::White);
    }

    updateText();
}

void Timer::updateText() {
    int minutes = timeRemaining / 60;
    int seconds = timeRemaining % 60;

    timeText.setString(
        (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds)
    );
}

void Timer::render(sf::RenderWindow& window) const {
    window.draw(timeText);
}


bool Timer::isTimeUp() const {
    return timeRemaining <= 0;
}

void Timer::reset(int initialTime) {
    timeRemaining = initialTime;
    clock.restart();
}
