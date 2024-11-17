#include "question.h"

Question::Question(const std::string& question,
                   const std::vector<std::string>& options,
                   int correctAnswer) :
        question(question),
        options(options),
        correctAnswer(correctAnswer)
{}

std::string Question::getQuestionText() const {
    return question;
}

std::vector<std::string> Question::getOptions() const {
    return options;
}

bool Question::checkAnswer(int answerIndex) const {
    return answerIndex == correctAnswer;
}
std::vector<Question> Question::getQuestions() {
    return {
        Question("Ce NU ar trebui să mănânce pisicile?", {"Sarmale", "Ciocolată", "Morcovi"}, 1),
        Question("Ce asemănare au ochii cameleonilor cu un obiectiv de cameră?", {"Pot mări ceea ce privesc", "Pot filma videoclipuri", "Pot fotografia peisaje"}, 0),
        Question("Cât de sus poate sări o căprioară? (aproximativ)", {"4 m", "2 m", "3 m"}, 2),
        Question("Care e mâncarea preferată a ursului koala?", {"Eucalipt", "Bambus", "Fructe exotice"}, 0),
        Question("Câte specii de maimuțe există la momentul actual?", {"80", "301", "267"}, 2),
        Question("În ce mediu natural trăiesc majoritatea papagalilor sălbatici??", {"Stepă și silvostepă", "Pădure tropicală", "Păduri de conifere"}, 1)
    };
}