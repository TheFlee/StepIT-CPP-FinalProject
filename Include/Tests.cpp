#include "Tests.h"

void loadTestsFromFile(const string& filename, vector<Category>& categories) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Could not open test file.\n";
        return;
    }

    string line;
    Category currentCategory;
    Test currentTest;
    Question currentQuestion;

    while (getline(inFile, line)) {
        if (line.rfind("Category: ", 0) == 0) {
            if (!currentQuestion.answers.empty()) {
                currentTest.questions.push_back(currentQuestion);
                currentQuestion = Question();
            }
            if (!currentTest.name.empty()) {
                currentCategory.tests.push_back(currentTest);
                currentTest = Test();
            }
            if (!currentCategory.name.empty() || !currentCategory.tests.empty()) {
                categories.push_back(currentCategory);
                currentCategory = Category();
            }
            currentCategory.name = line.substr(10);
        }
        else if (line.rfind("Test: ", 0) == 0) {
            if (!currentQuestion.answers.empty()) {
                currentTest.questions.push_back(currentQuestion);
                currentQuestion = Question();
            }
            if (!currentTest.name.empty()) {
                currentCategory.tests.push_back(currentTest);
                currentTest = Test();
            }
            currentTest.name = line.substr(6);
        }
        else if (line.rfind("Question: ", 0) == 0) {
            if (!currentQuestion.answers.empty()) {
                currentTest.questions.push_back(currentQuestion);
                currentQuestion = Question();
            }
            currentQuestion.text = line.substr(10);
        }
        else if (line.rfind("Answer: ", 0) == 0) {
            size_t delim = line.find(" | ");
            string answerText = line.substr(8, delim - 8);
            bool isCorrect = (line.substr(delim + 3) == "true");
            currentQuestion.answers.push_back({ answerText, isCorrect });
        }
    }

    if (!currentQuestion.answers.empty()) currentTest.questions.push_back(currentQuestion);
    if (!currentTest.name.empty()) currentCategory.tests.push_back(currentTest);
    if (!currentCategory.name.empty()) categories.push_back(currentCategory);
}
