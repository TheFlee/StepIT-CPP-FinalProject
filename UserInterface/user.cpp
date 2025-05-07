#include "user.h"

vector<Category> allCategories;

User::User(string fullName, string address, string phoneNumber,
    string login, string password)
    : fullName(fullName), address(address),
    phoneNumber(phoneNumber), login(login), password(password) {
}

string xorEncrypt(const string& input, char key = 'K') {
    string output = input;
    for (char& c : output) {
        c ^= key;
    }
    return output;
}

void User::registerUser() {
    cout << "Enter full name: "; 
    getline(cin, fullName);
    cout << "Enter address: "; 
    getline(cin, address);
    cout << "Enter phone number: "; 
    getline(cin, phoneNumber);

    string login;
    string encryptedLogin;
    while (true) {
        cout << "Choose login: ";
        getline(cin, login);
        encryptedLogin = xorEncrypt(login);

        ifstream inFile("../Include/users.txt");
        bool exists = false;
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                vector<string> fields;
                string field;
                istringstream ss(line);
                while (getline(ss, field, ',')) {
                    fields.push_back(field);
                }
                if (fields.size() >= 4 && fields[3] == encryptedLogin) {
                    exists = true;
                    break;
                }
            }
        }

        if (exists) {
            cout << "That login is already taken. Please choose another." << endl;
        }
        else {
            this->login = login;
            break;
        }
    }

    cout << "Choose password: ";
    getline(cin, password);
    string encryptedPassword = xorEncrypt(password);

    ofstream outFile("../Include/users.txt", ios::app);
    if (outFile.is_open()) {
        outFile << fullName << "," << address << "," << phoneNumber << "," << encryptedLogin << "," << encryptedPassword << endl;
    }
}

void User::loginUser() {
    string inputLogin, inputPassword;
    cout << "Enter login: "; getline(cin, inputLogin);
    cout << "Enter password: "; getline(cin, inputPassword);

    string encryptedLogin = xorEncrypt(inputLogin);
    string encryptedPassword = xorEncrypt(inputPassword);

    ifstream inFile("../Include/users.txt", ios::in);
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string fullName, address, phoneNumber, storedLogin, storedPassword;
        getline(ss, fullName, ',');
        getline(ss, address, ',');
        getline(ss, phoneNumber, ',');
        getline(ss, storedLogin, ',');
        getline(ss, storedPassword, ',');

        if (storedLogin == encryptedLogin && storedPassword == encryptedPassword) {
            this->fullName = fullName;
            this->address = address;
            this->phoneNumber = phoneNumber;
            login = storedLogin;
            password = storedPassword;
            loggedIn = true;
            cout << "Login successful." << endl;
            return;
        }
    }
    cout << "Login failed. Invalid credentials." << endl;
    loggedIn = false;
}

void User::takeTest() {
    string savedCategory, savedTest;
    int savedIndex = 0, savedScore = 0;
    bool hasProgress = loadProgress(savedCategory, savedTest, savedIndex, savedScore);

    Category* selectedCategoryPtr = nullptr;
    Test* selectedTestPtr = nullptr;
    int currentIndex = 0;
    int score = 0;

    if (hasProgress) {
        cout << "You have an unfinished test: '" << savedTest << "' in category '" << savedCategory << "'." << endl;
        cout << "1) Continue\n2) Discard and start a new test\nChoose: ";
        int contChoice;
        cin >> contChoice;
        cin.ignore();

        if (contChoice == 1) {
            for (Category& cat : allCategories) {
                if (cat.name == savedCategory) {
                    selectedCategoryPtr = &cat;
                    for (Test& tst : cat.tests) {
                        if (tst.name == savedTest) {
                            selectedTestPtr = &tst;
                            system("cls");
                            break;
                        }
                    }
                    break;
                }
            }
            if (!selectedCategoryPtr || !selectedTestPtr) {
                cout << "Saved test not found. Starting from scratch." << endl;
            }
            else {
                currentIndex = savedIndex;
                score = savedScore;
            }
        }
        else {
            deleteProgress();
        }
    }

    if (!selectedTestPtr) {
        if (allCategories.empty()) {
            cout << "No tests available." << endl;
            return;
        }

        cout << "Available Categories:" << endl;
        for (size_t i = 0; i < allCategories.size(); ++i) {
            cout << i + 1 << ") " << allCategories[i].name << endl;
        }

        int catChoice;
        cout << "Choose a category: ";
        cin >> catChoice;
        cin.ignore();
        system("cls");

        if (catChoice < 1 || catChoice > allCategories.size()) {
            cout << "Invalid choice." << endl;
            return;
        }

        selectedCategoryPtr = &allCategories[catChoice - 1];

        if (selectedCategoryPtr->tests.empty()) {
            cout << "No tests in this category." << endl;
            return;
        }

        cout << "Available Tests in '" << selectedCategoryPtr->name << "':" << endl;
        for (size_t i = 0; i < selectedCategoryPtr->tests.size(); ++i) {
            cout << i + 1 << ") " << selectedCategoryPtr->tests[i].name << endl;
        }

        int testChoice;
        cout << "Choose a test: ";
        cin >> testChoice;
        cin.ignore();
        system("cls");

        if (testChoice < 1 || testChoice > selectedCategoryPtr->tests.size()) {
            cout << "Invalid choice." << endl;
            return;
        }

        selectedTestPtr = &selectedCategoryPtr->tests[testChoice - 1];
        currentIndex = 0;
        score = 0;
    }

    Test& selectedTest = *selectedTestPtr;
    Category& selectedCategory = *selectedCategoryPtr;

    for (size_t i = currentIndex; i < selectedTest.questions.size(); ++i) {
        system("cls");
        cout << "Q" << i + 1 << ": " << selectedTest.questions[i].text << endl;
        for (size_t j = 0; j < selectedTest.questions[i].answers.size(); ++j) {
            cout << j + 1 << ") " << selectedTest.questions[i].answers[j].text << endl;
        }

        int userChoice;
        cout << "Enter your answer (number) or 0 to save & exit: ";
        cin >> userChoice;
        cin.ignore();

        if (userChoice == 0) {
            saveProgress(selectedCategory.name, selectedTest.name, i, score);
            cout << "Progress saved. You can continue later." << endl;
            return;
        }

        if (userChoice >= 1 && userChoice <= selectedTest.questions[i].answers.size() &&
            selectedTest.questions[i].answers[userChoice - 1].isCorrect) {
            score++;
        }

        saveProgress(selectedCategory.name, selectedTest.name, i + 1, score);
    }
    system("cls");
    cout << "Test Results:" << endl;
    cout << "Correct Answers: " << score << "/" << selectedTest.questions.size() << endl;
    cout << "Score: " << (score * 12) / selectedTest.questions.size() << "/12" << endl;
    cout << "Percentage: " << (static_cast<double>(score) / selectedTest.questions.size()) * 100 << "%" << endl;

    ofstream resultFile("../Include/" + fullName + "_results.txt", ios::app);
    if (resultFile.is_open()) {
        resultFile << "Category: " << selectedCategory.name
            << " | Test: " << selectedTest.name
            << " | Correct Answers: " << score << "/" << selectedTest.questions.size()
            << " | Score: " << (score * 12) / selectedTest.questions.size() << "/12"
            << " | Percentage: " << (static_cast<double>(score) / selectedTest.questions.size()) * 100 << "%" << endl;
        resultFile.close();
    }

    deleteProgress();
}

void User::viewResults() {
    ifstream inFile("../Include/" + fullName + "_results.txt", ios::in);
    if (!inFile.is_open()) {
        cout << "No results found for user." << endl;
        return;
    }

    vector<CategoryResult> results;
    string line;

    while (getline(inFile, line)) {
        size_t catPos = line.find("Category: ");
        size_t testPos = line.find(" | Test: ");
        size_t correctAnswersPos = line.find(" | Correct Answers: ");
        size_t scorePos = line.find(" | Score: ");
        size_t percentagePos = line.find(" | Percentage: ");

        if (catPos == string::npos || testPos == string::npos || correctAnswersPos == string::npos || scorePos == string::npos || percentagePos == string::npos)
            continue;

        string category = line.substr(catPos + 10, testPos - (catPos + 10));
        string testName = line.substr(testPos + 9, correctAnswersPos - (testPos + 9));
        string correctAnswers = line.substr(correctAnswersPos + 20, scorePos - (correctAnswersPos + 20));
        string score = line.substr(scorePos + 9, percentagePos - (scorePos + 9));
        string percentage = line.substr(percentagePos + 14);

        string attemptSummary = "Correct Answers: " + correctAnswers + " | Score: " + score + " | Percentage: " + percentage;

        bool categoryFound = false;
        for (auto& categoryResult : results) {
            if (categoryResult.categoryName == category) {
                bool testFound = false;
                for (auto& testResult : categoryResult.tests) {
                    if (testResult.testName == testName) {
                        testResult.attempts.push_back(attemptSummary);
                        testFound = true;
                        break;
                    }
                }
                if (!testFound) {
                    categoryResult.tests.push_back({ testName, {attemptSummary} });
                }
                categoryFound = true;
                break;
            }
        }

        if (!categoryFound) {
            CategoryResult newCategory = { category, { { testName, {attemptSummary} } } };
            results.push_back(newCategory);
        }
    }

    for (const auto& categoryResult : results) {
        cout << "\nCategory: " << categoryResult.categoryName << endl;
        for (const auto& testResult : categoryResult.tests) {
            cout << "  Test: " << testResult.testName << endl;
            for (size_t i = 0; i < testResult.attempts.size(); ++i) {
                cout << "    Attempt " << i + 1 << ": " << testResult.attempts[i] << endl;
            }
        }
    }
}


void User::saveProgress(const string& category, const string& test, int questionIndex, int score) {
    ofstream outFile("../Include/" + fullName + "_progress.txt");
    if (outFile.is_open()) {
        outFile << category << endl;
        outFile << test << endl;
        outFile << questionIndex << endl;
        outFile << score << endl;
    }
}

bool User::loadProgress(string& category, string& test, int& questionIndex, int& score) {
    ifstream inFile("../Include/" + fullName + "_progress.txt");
    if (!inFile.is_open()) return false;

    getline(inFile, category);
    getline(inFile, test);
    inFile >> questionIndex;
    inFile >> score;
    inFile.ignore();
    return true;
}

void User::deleteProgress() {
    remove(("../Include/" + fullName + "_progress.txt").c_str());
}