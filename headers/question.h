#pragma once

#include <memory>

#include "iquestion.h"
#include <string>
#include <unordered_map>
#include <vector>

class Question : public IQuestion {
protected:
    std::string questionText;

public:
    explicit Question(const std::string& questionText);
    virtual ~Question() = default;

    const std::string& getQuestionText() const override;
};

class ThreeOptionsQuestion : public Question {
    std::vector<std::string> options;
    int correctAnswer;

public:
    ThreeOptionsQuestion(const std::string& questionText, const std::vector<std::string>& options, int correctAnswer);
    bool checkAnswer(int answerIndex) const override;
    const std::vector<std::string>& getOptions() const override;
};

class TrueFalseQuestion : public Question {
    std::vector<std::string> options;
    bool correctAnswer;

public:
    TrueFalseQuestion(const std::string& questionText, bool correctAnswer);
    bool checkAnswer(int answerIndex) const override;
    const std::vector<std::string>& getOptions() const override;
};

class QuestionFactory {
public:
    static std::unordered_map<std::string, std::unique_ptr<IQuestion>> animalToQuestionMap;

    static void loadQuestionsFromFile(const std::string& threeQuestionsFile, const std::string& trueFalseQuestionsFile);
};
