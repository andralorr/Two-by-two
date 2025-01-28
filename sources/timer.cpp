#include "../headers/timer.h"
#include <iostream>
#include "../headers/gameexception.h"

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
    timeText.setCharacterSize(32);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Timer");
    sf::Vector2u windowSize = window.getSize();
    float centerX = static_cast<float>(windowSize.x) / 2.f;
    timeText.setPosition(centerX - 40.f, 130.f);
    clock.restart();
}

Timer::~Timer() {
    std::cout << "Destructor called for Timer" << std::endl;
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
