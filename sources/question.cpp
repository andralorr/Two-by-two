#include "../headers/question.h"

std::unordered_map<std::string, Question> Question::animalToQuestionMap;

Question::Question(const std::string& question,
                   const std::vector<std::string>& options,
                   int correctAnswer) :
        question(question),
        options(options),
        correctAnswer(correctAnswer)
{}

Question::Question(const Question& other) :
    question(other.question),
    options(other.options),
    correctAnswer(other.correctAnswer)
{}

Question::~Question() = default;

const std::string& Question::getQuestionText() const {
    return question;
}

const std::vector<std::string>& Question::getOptions() const {
    return options;
}


bool Question::checkAnswer(int answerIndex) const {
    return answerIndex == correctAnswer;
}


void Question::initializeQuestions() {
    static std::vector<Question> questions = {
        Question("What special feature allows owls to hunt so well at night?", {"They glow in the dark", "Silent flight", "X-ray vision"}, 1),
        Question("What is the most impressive trick chameleons can do?", {"Change color to blend with their environment","Change color to match their food", "Change color to communicate with aliens"}, 0),
        Question("What do male deer grow and shed every year?", {"Tail", "A mustache", "Antlers"}, 2),
        Question("How many bones are in a giraffe's neck?", {"7", "14", "21"}, 0),
        Question("What do monkeys use their tails for?", {"As a pillow", "To swat flies", "For balance and grabbing things"}, 2),
        Question("What unique talent are parrots famous for?", {"Doing backflips", "Mimicking sounds and speech", "Solving math problems"}, 1)
    };

    animalToQuestionMap["owl"] = questions[0];
    animalToQuestionMap["chameleon"] = questions[1];
    animalToQuestionMap["deer"] = questions[2];
    animalToQuestionMap["giraffe"] = questions[3];
    animalToQuestionMap["monkey"] = questions[4];
    animalToQuestionMap["parrot"] = questions[5];
}

Question* Question::getQuestionForAnimal(const std::string& animal) {
    auto it = animalToQuestionMap.find(animal);
    if (it != animalToQuestionMap.end()) {
        return &it->second;
    }
    return nullptr;
}

Question& Question::operator=(const Question& other) {
    if (this != &other) {
        question = other.question;
        options = other.options;
        correctAnswer = other.correctAnswer;
    }
    return *this;
}