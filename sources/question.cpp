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
        Question("What foods are toxic or dangerous for cats?", {"Tuna", "Garlic", "Spinach"}, 1),
        Question("What is the most impressive trick chameleons can do?", {"Change color to match their food", "Change color to blend with their environment", "Change color to communicate with aliens"}, 0),
        Question("How high can a deer jump?", {"Up to 4 m", "Up to 2 m", "Up to 3 m"}, 2),
        Question("What do koalas like to eat the most?", {"Eucalyptus", "Bamboo", "Passion Fruit"}, 0),
        Question("What is the total number of monkey species in the world today?", {"80", "301", "267"}, 2),
        Question("What is the primary natural environment of wild parrots?", {"Steppe and forest-steppe", "Tropical forest", "Coniferous forest"}, 1)
    };

    animalToQuestionMap["cat"] = questions[0];
    animalToQuestionMap["chameleon"] = questions[1];
    animalToQuestionMap["deer"] = questions[2];
    animalToQuestionMap["koala"] = questions[3];
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