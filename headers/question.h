#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include <unordered_map>

class Question {
private:
    std::string question;
    std::vector<std::string> options;
    int correctAnswer=-1;
    static std::unordered_map<std::string, Question> animalToQuestionMap;

public:
    Question() = default;
    Question(const std::string& question, const std::vector<std::string>& options, int correctAnswer);
    Question(const Question& other);
    Question& operator=(const Question& other);
    ~Question();
    bool checkAnswer(int answerIndex) const;
    const std::string& getQuestionText() const;
    const std::vector<std::string>& getOptions() const;

    static void initializeQuestions();
    static Question* getQuestionForAnimal(const std::string& animal);
};

#endif // QUESTION_H
