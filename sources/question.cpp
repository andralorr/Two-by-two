#include "../headers/question.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::unordered_map<std::string, std::unique_ptr<Question>> QuestionFactory::animalToQuestionMap;

Question::Question(const std::string& questionText) : questionText(questionText) {}

const std::string& Question::getQuestionText() const {
    return questionText;
}

ThreeOptionsQuestion::ThreeOptionsQuestion(const std::string& questionText,
                                           const std::vector<std::string>& options,
                                           int correctAnswer)
    : Question(questionText), options(options), correctAnswer(correctAnswer) {}

bool ThreeOptionsQuestion::checkAnswer(int answerIndex) const {
    return answerIndex == correctAnswer;
}

std::unique_ptr<Question> ThreeOptionsQuestion::clone() const {
    return std::make_unique<ThreeOptionsQuestion>(*this);
}

const std::vector<std::string>& ThreeOptionsQuestion::getOptions() const {
    return options;
}

TrueFalseQuestion::TrueFalseQuestion(const std::string& questionText, bool correctAnswer)
    : Question(questionText), correctAnswer(correctAnswer) {}

bool TrueFalseQuestion::checkAnswer(int answerIndex) const {
    return answerIndex == correctAnswer;
}

std::unique_ptr<Question> TrueFalseQuestion::clone() const {
    return std::make_unique<TrueFalseQuestion>(*this);
}

const std::vector<std::string>& TrueFalseQuestion::getOptions() const {
    return options;
}

void QuestionFactory::loadQuestionsFromFile(const std::string& filePath) {
    animalToQuestionMap.clear();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string type = line.substr(5);

        std::getline(file, line);
        std::string animal = line;

        std::getline(file, line);
        std::string question = line;

        if (type == "three") {
            std::getline(file, line);
            std::vector<std::string> options;
            std::stringstream optionsStream(line);
            std::string option;
            while (std::getline(optionsStream, option, '|')) {
                options.push_back(option);
            }

            std::getline(file, line);
            int correctAnswer = std::stoi(line);

            animalToQuestionMap[animal] = std::make_unique<ThreeOptionsQuestion>(question, options, correctAnswer);
        } else if (type == "truefalse") {
            std::getline(file, line);
            std::vector<std::string> options;
            std::stringstream optionsStream(line);
            std::string option;
            while (std::getline(optionsStream, option, '|')) {
                options.push_back(option);
            }

            std::getline(file, line);
            bool correctAnswer = (line == "1");

            animalToQuestionMap[animal] = std::make_unique<TrueFalseQuestion>(question, correctAnswer);
        }
    }

    file.close();
}
