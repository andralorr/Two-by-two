#include "../headers/timer.h"
#include <iostream>
#include "../headers/gameexception.h"
constexpr float TIMER_X_POS = 10.f;
constexpr float TIMER_Y_POS = 10.f;

Timer::Timer(int startTime) {
    timeRemaining = startTime;

    if (!font.loadFromFile("font/font.ttf")) {
        throw FileNotFoundException("Timer font");
    }
    if (!clockSoundBuffer.loadFromFile("music/clock.wav")) {
        throw FileNotFoundException("Clock sound");
    }
    clockSound.setBuffer(clockSoundBuffer);
    clockSound.setLoop(true);
    clockSound.setVolume(35);

    timeText.setFont(font);
    timeText.setCharacterSize(42);
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
        if (clockSound.getStatus() != sf::Sound::Playing) {
            clockSound.play();
        }
    } else if (timeRemaining > 30 && timeText.getFillColor() != sf::Color::White) {
        timeText.setFillColor(sf::Color::White);
        if (clockSound.getStatus() == sf::Sound::Playing) {
            clockSound.stop();
        }
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
    if (clockSound.getStatus() == sf::Sound::Playing) {
        clockSound.stop();
    }
}

bool Timer::isClockSoundPlaying() const {
    return clockSound.getStatus() == sf::Sound::Playing;
}

void Timer::stopClockSound() {
    if (clockSound.getStatus() == sf::Sound::Playing) {
        clockSound.stop();
    }
}
