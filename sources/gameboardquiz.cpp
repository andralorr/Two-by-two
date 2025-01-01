#include "../headers/gameboardquiz.h"
#include "../headers/gameexception.h"

GameBoardQuiz::GameBoardQuiz()
= default;

GameBoardQuiz::GameBoardQuiz(const GameBoardQuiz& other) :
    backgroundTexture(other.backgroundTexture),
    backgroundSprite(other.backgroundSprite),
    currentQuestion(other.currentQuestion),
    font(other.font),
    questionText(other.questionText),
    optionTexts(other.optionTexts),
    optionBoxes(other.optionBoxes)
{}

GameBoardQuiz::~GameBoardQuiz() = default;

void GameBoardQuiz::createWindow() {
    if (!quizWindow.isOpen()) {
        quizWindow.create(sf::VideoMode(1100, 600), "Question", sf::Style::Titlebar);

        quizWindow.setVerticalSyncEnabled(true);

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        sf::Vector2i position(
            (desktop.width - 1100) / 2,
            (desktop.height - 600) / 2
        );
        quizWindow.setPosition(position);
    }

    if (!backgroundTexture.loadFromFile("Images/assets/quizbackground.png")) {
        throw FileNotFoundException("quizbackground");
    }
    backgroundSprite.setTexture(backgroundTexture);

    float scaleX = static_cast<float>(quizWindow.getSize().x) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(quizWindow.getSize().y) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    if (!font.loadFromFile("font/font.ttf")) {
        throw FileNotFoundException("quiz font");
    }

    questionText.setFont(font);
    questionText.setCharacterSize(24);
    questionText.setFillColor(sf::Color(100, 100, 255));
    questionText.setStyle(sf::Text::Bold);
}

void GameBoardQuiz::setCurrentQuestion(Question* question) {
    currentQuestion = question;
    positionQuestions();
}

void GameBoardQuiz::positionQuestions() {
    optionTexts.clear();
    optionBoxes.clear();

    const int windowWidth = quizWindow.getSize().x;

    if (currentQuestion == nullptr) {
        questionText.setString("No question available (gameboardquiz)");
        sf::FloatRect textBounds = questionText.getLocalBounds();
        questionText.setPosition(
            (windowWidth - textBounds.width) / 2,
            80
        );
        return;
    }

    questionText.setString(currentQuestion->getQuestionText());
    sf::FloatRect questionBounds = questionText.getLocalBounds();
    questionText.setPosition(
        (windowWidth - questionBounds.width) / 2,
        100
    );

    const float optionBoxWidth = static_cast<float>(windowWidth) * 0.5f;
    const float optionBoxHeight = 50.0f;
    const float spacing = 20.0f;
    const float startY = 180.0f;

    std::vector<std::string> options = currentQuestion->getOptions();
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setCharacterSize(20);
        optionText.setFillColor(sf::Color::White);
        optionText.setString(options[i]);

        sf::RectangleShape optionBox;
        optionBox.setSize(sf::Vector2f(optionBoxWidth, optionBoxHeight));
        optionBox.setFillColor(sf::Color(100, 100, 255));

        float boxX = (windowWidth - optionBoxWidth) / 2;
        float boxY = startY + i * (optionBoxHeight + spacing);
        optionBox.setPosition(boxX, boxY);

        sf::FloatRect textBounds = optionText.getLocalBounds();
        float textX = boxX + (optionBoxWidth - textBounds.width) / 2;
        float textY = boxY + (optionBoxHeight - textBounds.height) / 2 - textBounds.top;
        optionText.setPosition(textX, textY);

        optionTexts.push_back(optionText);
        optionBoxes.push_back(optionBox);
    }
}

void GameBoardQuiz::render() {
    quizWindow.clear();
    quizWindow.draw(backgroundSprite);
    quizWindow.draw(questionText);

    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        quizWindow.draw(optionBoxes[i]);
        quizWindow.draw(optionTexts[i]);
    }

    quizWindow.display();
}

sf::RenderWindow& GameBoardQuiz::getWindowQuiz() {
    return quizWindow;
}

int GameBoardQuiz::getOptionAtPosition(const sf::Vector2i& position) const {
    sf::Vector2f worldPos = quizWindow.mapPixelToCoords(position);
    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(worldPos)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

GameBoardQuiz& GameBoardQuiz::operator=(const GameBoardQuiz& other) {
    if (this != &other) {
        backgroundTexture = other.backgroundTexture;
        backgroundSprite = other.backgroundSprite;
        font = other.font;
        questionText = other.questionText;
        optionTexts = other.optionTexts;
        optionBoxes = other.optionBoxes;
        currentQuestion = other.currentQuestion;
    }
    return *this;
}
