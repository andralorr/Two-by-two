#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

class Question {
private:
    std::string question;
    std::vector<std::string> options;
    int correctAnswer;
public:
    Question(const std::string& question, const std::vector<std::string>& options, int correctAnswer);

    bool checkAnswer(int answerIndex) const;

    static std::vector<Question> getQuestions();
    std::string getQuestionText() const;
    std::vector<std::string> getOptions() const;
};
#endif //QUESTION_H
