#include "../headers/question.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

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


const std::vector<std::string>& ThreeOptionsQuestion::getOptions() const {
    return options;
}

TrueFalseQuestion::TrueFalseQuestion(const std::string& questionText, bool correctAnswer)
    : Question(questionText),options{"False", "True"}, correctAnswer(correctAnswer) {
}

bool TrueFalseQuestion::checkAnswer(int answerIndex) const {
    return static_cast<bool>(answerIndex) == correctAnswer;
}

const std::vector<std::string>& TrueFalseQuestion::getOptions() const {
    return options;
}

void QuestionFactory::loadQuestionsFromFile(const std::string& threeQuestionsFile, const std::string& trueFalseQuestionsFile) {
    animalToQuestionMap.clear();
    loadThreeOptionsQuestions(threeQuestionsFile);
    loadTrueFalseQuestions(trueFalseQuestionsFile);
    std::cout << "Questions loaded successfully.\n";
}

void QuestionFactory::loadThreeOptionsQuestions(const std::string& filePath) {
    std::cout << "Loading three-options questions from: " << filePath << std::endl;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string animal = line;
        std::cout << "Reading animal name: " << animal << std::endl;

        std::getline(file, line);
        std::string question = line;
        if (question.empty()) {
            throw std::runtime_error("Missing question text for animal: " + animal);
        }
        std::cout << "Question: " << question << std::endl;

        std::getline(file, line);
        std::vector<std::string> options;
        std::stringstream optionsStream(line);
        std::string option;
        while (std::getline(optionsStream, option, '|')) {
            if (option.empty()) {
                throw std::runtime_error("Empty option found for animal: " + animal);
            }
            options.push_back(option);
        }
        if (options.size() != 3) {
            throw std::runtime_error("Invalid number of options for animal: " + animal + ". Expected 3.");
        }
        std::cout << "Options: ";
        for (const auto& opt : options) std::cout << opt << " ";
        std::cout << std::endl;

        std::getline(file, line);
        int correctAnswer = std::stoi(line);
        if (correctAnswer < 0 || correctAnswer >= static_cast<int>(options.size())) {
            throw std::runtime_error("Invalid correct answer index for animal: " + animal);
        }

        animalToQuestionMap[animal] = std::make_unique<ThreeOptionsQuestion>(question, options, correctAnswer);
        std::cout << "Added question for animal: " << animal << std::endl;
    }

    file.close();
    std::cout << "Finished loading three-options questions.\n";
}

void QuestionFactory::loadTrueFalseQuestions(const std::string& filePath) {
    std::cout << "Loading true/false questions from: " << filePath << std::endl;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string animal = line;
        std::cout << "Reading animal name: " << animal << std::endl;

        std::getline(file, line);
        std::string question = line;
        if (question.empty()) {
            throw std::runtime_error("Missing question text for animal: " + animal);
        }
        std::cout << "Question: " << question << std::endl;

        std::getline(file, line);
        if (line != "0" && line != "1") {
            throw std::runtime_error("Invalid correct answer for animal: " + animal + ". Must be 0 or 1.");
        }
        bool correctAnswer = (line == "1");

        animalToQuestionMap[animal] = std::make_unique<TrueFalseQuestion>(question, correctAnswer);
        std::cout << "Added question for animal: " << animal << std::endl;
    }

    file.close();
    std::cout << "Finished loading true/false questions.\n";
}






