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
    cout << "Enter full name: "; getline(cin, fullName);
    cout << "Enter address: "; getline(cin, address);
    cout << "Enter phone number: "; getline(cin, phoneNumber);

    string candidateLogin;
    string encryptedLogin;
    while (true) {
        cout << "Choose login: ";
        getline(cin, candidateLogin);
        encryptedLogin = xorEncrypt(candidateLogin);

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
            cout << "That login is already taken. Please choose another.\n";
        }
        else {
            login = candidateLogin;
            break;
        }
    }

    cout << "Choose password: ";
    getline(cin, password);
    string encryptedPassword = xorEncrypt(password);

    ofstream outFile("../Include/users.txt", ios::app);
    if (outFile.is_open()) {
        outFile << fullName << "," << address << "," << phoneNumber << ","
            << encryptedLogin << "," << encryptedPassword << endl;
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
        string fName, addr, phone, storedLogin, storedPassword;
        getline(ss, fName, ',');
        getline(ss, addr, ',');
        getline(ss, phone, ',');
        getline(ss, storedLogin, ',');
        getline(ss, storedPassword, ',');

        if (storedLogin == encryptedLogin && storedPassword == encryptedPassword) {
            fullName = fName;
            address = addr;
            phoneNumber = phone;
            login = storedLogin;
            password = storedPassword;
            loggedIn = true;
            cout << "Login successful.\n";
            return;
        }
    }
    cout << "Login failed. Invalid credentials.\n";
    loggedIn = false;
}

void User::takeTest() {
    #pragma region FileChecking

    if (allCategories.empty()) {
        cout << "No tests available.\n";
        return;
    }

    cout << "\nAvailable Categories:\n";
    for (size_t i = 0; i < allCategories.size(); ++i) {
        cout << i + 1 << ") " << allCategories[i].name << endl;
    }

    int catChoice;
    cout << "Choose a category: ";
    cin >> catChoice;
    cin.ignore();

    if (catChoice < 1 || catChoice > allCategories.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    Category& selectedCategory = allCategories[catChoice - 1];

    if (selectedCategory.tests.empty()) {
        cout << "No tests in this category.\n";
        return;
    }

    cout << "\nAvailable Tests in '" << selectedCategory.name << "':\n";
    for (size_t i = 0; i < selectedCategory.tests.size(); ++i) {
        cout << i + 1 << ") " << selectedCategory.tests[i].name << endl;
    }

    int testChoice;
    cout << "Choose a test: ";
    cin >> testChoice;
    cin.ignore();

    if (testChoice < 1 || testChoice > selectedCategory.tests.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    Test& selectedTest = selectedCategory.tests[testChoice - 1];
#pragma endregion

    int score = 0;
    for (size_t i = 0; i < selectedTest.questions.size(); ++i) {
        cout << "\nQ" << i + 1 << ": " << selectedTest.questions[i].text << endl;
        for (size_t j = 0; j < selectedTest.questions[i].answers.size(); ++j) {
            cout << j + 1 << ") " << selectedTest.questions[i].answers[j].text << endl;
        }

        int userChoice;
        cout << "Enter your answer (number): ";
        cin >> userChoice;
        cin.ignore();

        if (userChoice >= 1 && userChoice <= selectedTest.questions[i].answers.size() &&
            selectedTest.questions[i].answers[userChoice - 1].isCorrect) {
            score++;
        }
    }

    cout << "\nTest Results:\n";
    cout << "\nCorrect Answers " << score << "/" << selectedTest.questions.size() << endl;
    cout << "Score " << (score * 12) / selectedTest.questions.size() << "/12" << endl;
    cout << "Percentage " << (static_cast<double>(score) / selectedTest.questions.size()) * 100 << "%" << endl;

    ofstream resultFile("../Include/" + fullName + "_results.txt", ios::app);
    if (resultFile.is_open()) {
        resultFile << "Category: " << selectedCategory.name
            << " | Test: " << selectedTest.name
            << " | Correct Answers: " << score << "/" << selectedTest.questions.size()
            << " | Score: " << (score * 12) / selectedTest.questions.size() << "/12"
            << " | Percentage: " << (static_cast<double>(score) / selectedTest.questions.size()) * 100 << "%" << endl;
        resultFile.close();
    }
    else {
        cout << "Error saving result.\n";
    } 
}


void User::viewResults() {
    ifstream inFile("../Include/" + fullName + "_results.txt", ios::in);
    if (!inFile.is_open()) {
        cout << "No results found for user.\n";
        return;
    }

    vector<CategoryResult> results;
    string line;

    while (getline(inFile, line)) {
        // Expected format: Category: <category> | Test: <test name> | Score: x/y
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
                // Find test under this category
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

    // Display results
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
