#include "../headers/gameexception.h"

GameException::GameException(const std::string& msg) : message(msg) {}

const char* GameException::what() const noexcept {
    return message.c_str();
}

FileNotFoundException::FileNotFoundException(const std::string& filename)
    : GameException("File not found: " + filename) {}

InvalidStateException::InvalidStateException(const std::string& msg)
    : GameException("Invalid state: " + msg) {}

GameLogicException::GameLogicException(const std::string& msg)
    : GameException("Game logic error: " + msg) {}