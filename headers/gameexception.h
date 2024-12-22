#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg);
    const char* what() const noexcept override;
};

class FileNotFoundException : public GameException {
public:
    explicit FileNotFoundException(const std::string& filename);
};

class InvalidStateException : public GameException {
public:
    explicit InvalidStateException(const std::string& msg);
};

#endif // GAMEEXCEPTION_H
