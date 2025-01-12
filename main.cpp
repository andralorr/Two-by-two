#include "headers/game.h"
#include "headers/gameexception.h"
#include <SFML/Audio.hpp>
#include <iostream>

int main() {
    try {
        std::cout << "Initializing game...\n";
        QuestionFactory::loadQuestionsFromFile("Init/questions.txt", "Init/truefalse.txt");
        std::cout << "Questions loaded successfully.\n";

        std::cout << "Game starting...\n";
        Game game;

        sf::Music backgroundMusic;
        if (!backgroundMusic.openFromFile("music/music.wav")) {
            throw std::runtime_error("Failed to load background music!");
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(10);
        backgroundMusic.play();

        std::cout << "Game initialized successfully.\n";
        game.run();
    } catch (const FileNotFoundException& e) {
        std::cerr << "File error: " << e.what() << std::endl;
    } catch (const InvalidStateException& e) {
        std::cerr << "State error: " << e.what() << std::endl;
    } catch (const GameLogicException& e) {
        std::cerr << "Logic error: " << e.what() << std::endl;
    } catch (const GameException& e) {
        std::cerr << "Game error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
    return 0;
}
