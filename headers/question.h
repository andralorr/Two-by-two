#ifndef QUESTION_H
#define QUESTION_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <memory>

class Question {
protected:
    std::string questionText;

public:
    explicit Question(const std::string& questionText);
    virtual ~Question() = default;

    virtual bool checkAnswer(int answerIndex) const = 0;
    virtual std::unique_ptr<Question> clone() const = 0;

    const std::string& getQuestionText() const;
    virtual const std::vector<std::string>& getOptions() const = 0;
};

class ThreeOptionsQuestion : public Question {
    std::vector<std::string> options;
    int correctAnswer;

public:
    ThreeOptionsQuestion(const std::string& questionText,
                         const std::vector<std::string>& options,
                         int correctAnswer);
    bool checkAnswer(int answerIndex) const override;
    std::unique_ptr<Question> clone() const override;
    const std::vector<std::string>& getOptions() const override;
};

class TrueFalseQuestion : public Question {
    std::vector<std::string> options = {"True", "False"};
    bool correctAnswer;

public:
    TrueFalseQuestion(const std::string& questionText, bool correctAnswer);
    bool checkAnswer(int answerIndex) const override;
    std::unique_ptr<Question> clone() const override;
    const std::vector<std::string>& getOptions() const override;
};

class QuestionFactory {
public:
    static void loadQuestionsFromFile(const std::string& filePath);
    static std::unordered_map<std::string, std::unique_ptr<Question>> animalToQuestionMap;
};
#endif // QUESTION_H
