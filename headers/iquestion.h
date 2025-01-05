#ifndef IQUESTION_H
#define IQUESTION_H

#pragma once

#include <string>
#include <vector>

class IQuestion {
public:
    virtual ~IQuestion() = default;

    virtual const std::string& getQuestionText() const = 0;

    virtual bool checkAnswer(int answerIndex) const = 0;

    virtual const std::vector<std::string>& getOptions() const = 0;
};

#endif //IQUESTION_H
