#include "admin.h"

extern string xorEncrypt(const string& input, char key = 'K');
extern vector<Category> allCategories;

Admin::Admin(string login, string password)
    : login(login), password(password) {
}

void Admin::loginAdmin() {
    ifstream file("admin.txt");
    if (!file) {
        cout << "Admin file not found." << endl;
        return;
    }

    string storedLogin, storedPassword;
    getline(file, storedLogin);
    getline(file, storedPassword);
    file.close();

    string inputLogin, inputPassword;
    cout << "Enter admin login: ";
    cin >> inputLogin;
    cout << "Enter admin password: ";
    cin >> inputPassword;

    if (xorEncrypt(inputLogin) == storedLogin && xorEncrypt(inputPassword) == storedPassword) {
        loggedIn = true;
        login = storedLogin;
        password = storedPassword;
        cout << "Login successful!" << endl;
    }
    else {
        cout << "Invalid login or password." << endl;
    }
}

void Admin::changeCredentials() {
    string newLogin, newPassword;
    cout << "Enter new login: ";
    cin >> newLogin;
    cout << "Enter new password: ";
    cin >> newPassword;

    login = xorEncrypt(newLogin);
    password = xorEncrypt(newPassword);

    ofstream file("admin.txt");
    if (!file) {
        cout << "Error opening admin credentials file for writing." << endl;
        return;
    }

    file << login << endl << password << endl;
    file.close();

    cout << "Credentials updated successfully." << endl;
}


void Admin::createUser() {
    User newUser;
    cin.ignore();
    newUser.registerUser();
    cout << "User created successfully." << endl;
}

void Admin::deleteUser() {
    ifstream inFile("../Include/users.txt");
    if (!inFile) {
        cout << "Could not open users.txt\n";
        return;
    }

    vector<string> lines;
    vector<string> userLogins;

    string line;
    while (getline(inFile, line)) {
        lines.push_back(line);
        istringstream ss(line);
        string fn, ad, ph, login;
        getline(ss, fn, ',');
        getline(ss, ad, ',');
        getline(ss, ph, ',');
        getline(ss, login, ',');
        userLogins.push_back(login);
    }
    inFile.close();

    if (lines.empty()) {
        cout << "No users found." << endl;
        return;
    }

    cout << "\nUsers:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        istringstream ss(lines[i]);
        string name;
        getline(ss, name, ',');
        cout << i + 1 << ") " << name << endl;
    }

    int choice;
    cout << "Enter the number of the user to delete: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > userLogins.size()) {
        cout << "Invalid selection." << endl;
        return;
    }

    string targetLogin = userLogins[choice - 1];

    ofstream outFile("../Include/users.txt");
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i != choice - 1) {
            outFile << lines[i] << endl;
        }
    }

    cout << "User deleted successfully." << endl;
}

void Admin::editUser() {
    ifstream inFile("../Include/users.txt");
    if (!inFile) {
        cout << "Could not open users.txt" << endl;
        return;
    }

    vector<string> lines;
    vector<string> userLogins;

    string line;
    while (getline(inFile, line)) {
        lines.push_back(line);
        istringstream ss(line);
        string fn, ad, ph, login;
        getline(ss, fn, ',');
        getline(ss, ad, ',');
        getline(ss, ph, ',');
        getline(ss, login, ',');
        userLogins.push_back(login);
    }
    inFile.close();

    if (lines.empty()) {
        cout << "No users found." << endl;
        return;
    }

    cout << "\nUsers:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        istringstream ss(lines[i]);
        string name;
        getline(ss, name, ',');
        cout << i + 1 << ") " << name << endl;
    }

    int choice;
    cout << "Enter the number of the user to edit: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > userLogins.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    string encryptedLogin = userLogins[choice - 1];

    cout << "Enter new full name: ";
    string fullName;
    getline(cin, fullName);
    cout << "Enter new address: ";
    string address;
    getline(cin, address);
    cout << "Enter new phone number: ";
    string phone;
    getline(cin, phone);
    cout << "Enter new password: ";
    string password;
    getline(cin, password);
    string encryptedPassword = xorEncrypt(password);

    lines[choice - 1] = fullName + "," + address + "," + phone + "," + encryptedLogin + "," + encryptedPassword;

    ofstream outFile("../Include/users.txt");
    for (const string& l : lines) {
        outFile << l << endl;
    }

    cout << "User updated successfully." << endl;
}


void Admin::viewResultsByUser() {
    ifstream inFile("../Include/users.txt");
    if (!inFile) {
        cout << "Cannot open users file." << endl;
        return;
    }

    vector<string> users;
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string fullName;
        getline(ss, fullName, ',');
        users.push_back(fullName);
    }

    if (users.empty()) {
        cout << "No users found." << endl;
        return;
    }

    cout << "Select a user:" << endl;
    for (size_t i = 0; i < users.size(); ++i) {
        cout << i + 1 << ") " << users[i] << endl;
    }

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > users.size()) {
        cout << "Invalid selection." << endl;
        return;
    }

    string selectedUser = users[choice - 1];
    ifstream resultFile("../Include/" + selectedUser + "_results.txt");

    system("cls");

    if (!resultFile) {
        cout << "No results for this user." << endl;
        return;
    }

    cout << "Results for " << selectedUser << ":" << endl;
    while (getline(resultFile, line)) {
        cout << line << endl;
    }
}

void Admin::viewResultsByCategory() {
    ifstream userFile("../Include/users.txt");
    if (!userFile) {
        cout << "Failed to open users file." << endl;
        return;
    }

    map<string, vector<string>> categoryMap;

    string userLine;
    while (getline(userFile, userLine)) {
        stringstream ss(userLine);
        string fullName;
        getline(ss, fullName, ',');

        ifstream resultFile("../Include/" + fullName + "_results.txt");
        string resultLine;
        while (getline(resultFile, resultLine)) {
            size_t catPos = resultLine.find("Category: ");
            size_t testPos = resultLine.find(" | Test: ");
            if (catPos != string::npos && testPos != string::npos) {
                string category = resultLine.substr(catPos + 10, testPos - (catPos + 10));
                string cleanedLine = resultLine.substr(testPos + 9);
                categoryMap[category].push_back("User: " + fullName + " | " + cleanedLine);
            }
        }
    }

    for (auto i = categoryMap.begin(); i != categoryMap.end(); ++i) {
        cout << "Category: " << i->first << endl;
        for (const string& entry : i->second) {
            cout << "  " << entry << endl;
        }
    }
}

void Admin::viewResultsByTest() {
    ifstream userFile("../Include/users.txt");
    if (!userFile) {
        cout << "Failed to open users file." << endl;
        return;
    }

    map<string, vector<string>> testMap;

    string userLine;
    while (getline(userFile, userLine)) {
        stringstream ss(userLine);
        string fullName;
        getline(ss, fullName, ',');

        ifstream resultFile("../Include/" + fullName + "_results.txt");
        string resultLine;
        while (getline(resultFile, resultLine)) {
            size_t testPos = resultLine.find(" | Test: ");
            size_t correctPos = resultLine.find(" | Correct Answers: ");
            if (testPos != string::npos && correctPos != string::npos) {
                string test = resultLine.substr(testPos + 9, correctPos - (testPos + 9));
                string cleanedLine = resultLine.substr(correctPos + 3);
                testMap[test].push_back("User: " + fullName + " |" + cleanedLine);
            }
        }
    }
    for (auto i = testMap.begin(); i != testMap.end(); ++i) {
        cout << "Test: " << i->first << endl;
        for (const string& entry : i->second) {
            cout << "  " << entry << endl;
        }
    }
}

void Admin::exportStats() {
    ofstream outFile("../Include/all_stats_export.txt");
    if (!outFile) {
        cout << "Failed to create export file." << endl;
        return;
    }

    ifstream userFile("../Include/users.txt");
    vector<string> users;
    string line;

    while (getline(userFile, line)) {
        stringstream ss(line);
        string fullName;
        getline(ss, fullName, ',');
        users.push_back(fullName);
    }

    for (const string& user : users) {
        ifstream resultFile("../Include/" + user + "_results.txt");
        if (!resultFile) continue;

        outFile << "User: " << user << endl;
        while (getline(resultFile, line)) {
            outFile << "  " << line << endl;
        }
        outFile << "\n";
    }

    cout << "Exported statistics to all_stats_export.txt" << endl;
}


void Admin::addCategory() {
    string categoryName;
    cout << "Enter new category name: ";
    getline(cin, categoryName);

    for (const auto& cat : allCategories) {
        if (cat.name == categoryName) {
            cout << "Category '" << categoryName << "' already exists!" << endl;
            return;
        }
    }

    Category newCat;
    newCat.name = categoryName;
    allCategories.push_back(newCat);
    cout << "Category '" << categoryName << "' added successfully." << endl;

    exportTests();
}

void Admin::addTest() {
    if (allCategories.empty()) {
        cout << "No categories available. Add a category first." << endl;
        return;
    }

    cout << "Select category to add a test to:\n";
    for (size_t i = 0; i < allCategories.size(); ++i) {
        cout << "  " << (i + 1) << ") " << allCategories[i].name << endl;
    }
    cout << "Enter choice: ";
    size_t choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > allCategories.size()) {
        cout << "Invalid category choice." << endl;
        return;
    }

    Category& cat = allCategories[choice - 1];
    string testName;
    cout << "Enter new test name: ";
    getline(cin, testName);

    for (const auto& t : cat.tests) {
        if (t.name == testName) {
            cout << "Test '" << testName << "' already exists in category '" << cat.name << "'." << endl;
            return;
        }
    }

    Test newTest;
    newTest.name = testName;
    cat.tests.push_back(newTest);
    cout << "Test '" << testName << "' added to category '" << cat.name << "'." << endl;

    exportTests();
}

void Admin::addQuestion() {
    if (allCategories.empty()) {
        cout << "No categories available." << endl;
        return;
    }
    cout << "Select category:" << endl;
    for (size_t i = 0; i < allCategories.size(); ++i) {
        cout << "  " << (i + 1) << ") " << allCategories[i].name << endl;
    }
    cout << "Enter choice: ";
    size_t catChoice;
    cin >> catChoice;
    cin.ignore();
    if (catChoice < 1 || catChoice > allCategories.size()) {
        cout << "Invalid category choice.\n";
        return;
    }
    Category& cat = allCategories[catChoice - 1];

    if (cat.tests.empty()) {
        cout << "No tests in this category. Add a test first." << endl;
        return;
    }
    cout << "Select test in '" << cat.name << "':" << endl;
    for (size_t i = 0; i < cat.tests.size(); ++i) {
        cout << "  " << (i + 1) << ") " << cat.tests[i].name << endl;
    }
    cout << "Enter choice: ";
    size_t testChoice;
    cin >> testChoice;
    cin.ignore();
    if (testChoice < 1 || testChoice > cat.tests.size()) {
        cout << "Invalid test choice." << endl;
        return;
    }
    Test& test = cat.tests[testChoice - 1];

    cout << "How many questions do you want to add? ";
    size_t numQuestions;
    cin >> numQuestions;
    cin.ignore();

    for (size_t q = 0; q < numQuestions; ++q) {
        Question newQ;
        cout << "Enter text for question " << (q + 1) << ": ";
        getline(cin, newQ.text);
        for (int a = 0; a < 4; ++a) {
            Answer ans;
            cout << "Enter text for answer " << (a + 1) << ": ";
            getline(cin, ans.text);
            cout << "Is this answer correct? (1 = yes, 0 = no): ";
            int correct;
            cin >> correct;
            ans.isCorrect = (correct == 1);
            cin.ignore();
            newQ.answers.push_back(ans);
        }
        test.questions.push_back(newQ);
        cout << "Question " << (q + 1) << " added to test '" << test.name << "'." << endl;
    }

    exportTests();
}

void Admin::exportTests() {
    ofstream file("../Include/tests.txt");
    if (!file) {
        cout << "Failed to open tests.txt for writing." << endl;
        return;
    }
    for (const auto& cat : allCategories) {
        file << "Category: " << cat.name << endl;
        for (const auto& test : cat.tests) {
            file << "Test: " << test.name << endl;
            for (const auto& q : test.questions) {
                file << "Question: " << q.text << endl;
                for (const auto& a : q.answers) {
                    file << "Answer: " << a.text << " | "
                        << (a.isCorrect ? "true" : "false") << endl;
                }
            }
        }
    }
    cout << "Exported to tests.txt." << endl;
}

void Admin::importTests() {
    cout << "Enter file path: ";
    string filename;
    getline(cin, filename);
    ifstream file(filename);
    if (!file) {
        cout << filename <<" not found." << endl;
        return;
    }

    allCategories.clear();
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            lines.push_back(line);
    }

    Category* currentCat = nullptr;
    Test* currentTest = nullptr;

    for (size_t i = 0; i < lines.size(); ++i) {
        const string& l = lines[i];
        if (l.rfind("Category: ", 0) == 0) {
            string name = l.substr(10);
            allCategories.push_back({ name, {} });
            currentCat = &allCategories.back();
            currentTest = nullptr;
        }
        else if (l.rfind("Test: ", 0) == 0) {
            if (!currentCat) continue;
            string tname = l.substr(6);
            currentCat->tests.push_back({ tname, {} });
            currentTest = &currentCat->tests.back();
        }
        else if (l.rfind("Question: ", 0) == 0) {
            if (!currentTest) continue;
            Question q;
            q.text = l.substr(10);
            for (size_t j = i + 1; j < lines.size() && lines[j].rfind("Answer: ", 0) == 0; ++j) {
                size_t sep = lines[j].find(" | ");
                string atxt = lines[j].substr(8, sep - 8);
                bool correct = (lines[j].substr(sep + 3) == "true");
                q.answers.push_back({ atxt, correct });
                i = j;
            }
            currentTest->questions.push_back(q);
        }
    }

    cout << "Imported from " << filename << "." << endl;
}