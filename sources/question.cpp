#include <fstream>
#include <iostream>
#include <sstream>

#include "../headers/question.h"

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
    : Question(questionText), options({"False", "True"}), correctAnswer(correctAnswer) {}

bool TrueFalseQuestion::checkAnswer(int answerIndex) const {
    return static_cast<bool>(answerIndex) == correctAnswer;
}

const std::vector<std::string>& TrueFalseQuestion::getOptions() const {
    return options;
}

std::unordered_map<std::string, std::unique_ptr<IQuestion>> QuestionFactory::animalToQuestionMap;

void QuestionFactory::loadQuestionsFromFile(const std::string& threeQuestionsFile, const std::string& trueFalseQuestionsFile) {
    animalToQuestionMap.clear();

    std::ifstream threeFile(threeQuestionsFile);
    if (!threeFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + threeQuestionsFile);
    }

    std::string line;
    while (std::getline(threeFile, line)) {
        if (line.empty()) continue;

        std::string animal = line;
        std::getline(threeFile, line);
        std::string questionText = line;

        std::getline(threeFile, line);
        std::vector<std::string> options;
        std::stringstream ss(line);
        std::string option;
        while (std::getline(ss, option, '|')) {
            options.push_back(option);
        }

        std::getline(threeFile, line);
        int correctAnswer = std::stoi(line);

        animalToQuestionMap[animal] = std::make_unique<ThreeOptionsQuestion>(questionText, options, correctAnswer);
    }

    threeFile.close();

    std::ifstream tfFile(trueFalseQuestionsFile);
    if (!tfFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + trueFalseQuestionsFile);
    }

    while (std::getline(tfFile, line)) {
        if (line.empty()) continue;

        std::string animal = line;
        std::getline(tfFile, line);
        std::string questionText = line;

        std::getline(tfFile, line);
        bool correctAnswer = (line == "1");

        animalToQuestionMap[animal] = std::make_unique<TrueFalseQuestion>(questionText, correctAnswer);
    }

    tfFile.close();

    std::cout << "Questions loaded successfully.\n";
}

std::unique_ptr<IQuestion> QuestionFactory::createQuestion(const std::string& type,
                                                           const std::string& questionText,
                                                           const std::vector<std::string>& options,
                                                           int correctAnswer) {
    if (type == "ThreeOptions") {
        return std::make_unique<ThreeOptionsQuestion>(questionText, options, correctAnswer);
    } else if (type == "TrueFalse") {
        return std::make_unique<TrueFalseQuestion>(questionText, correctAnswer == 1);
    } else {
        throw std::invalid_argument("Unknown question type: " + type);
    }
}