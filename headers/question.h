#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include <unordered_map>

class Question {
private:
    std::string question;
    std::vector<std::string> options;
    int correctAnswer;
    static std::unordered_map<std::string, Question> animalToQuestionMap;

public:
    Question() = default;
    Question(const std::string& question, const std::vector<std::string>& options, int correctAnswer);
    bool checkAnswer(int answerIndex) const;
    std::string getQuestionText() const;
    std::vector<std::string> getOptions() const;
    void initializeQuestions();
    static Question* getQuestionForAnimal(const std::string& animal);
};

#endif // QUESTION_H
