#include "user.h"

void setColor(int);
void drawBox(const string&);

int main() {
    User currentUser;
    bool isAuthenticated = false;
    loadTestsFromFile("../Include/tests.txt", allCategories);

    while (!currentUser.isLoggedIn()) {
        system("cls");
        setColor(11);
        drawBox("Welcome");
        setColor(14);
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        setColor(7);
        cout << "Choose an option (1 or 2): ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            system("cls");
            setColor(13);
            drawBox("Register");
            setColor(7);
            currentUser.registerUser();
            cout << "Now please log in." << endl;
            _getch();
            break;
        case 2:
            system("cls");
            setColor(10);
            drawBox("Login");
            setColor(7);
            currentUser.loginUser();
            break;
        default:
            system("cls");
            setColor(12);
            drawBox("Error");
            setColor(7);
            cout << "Invalid choice. Please select 1 or 2." << endl;
            _getch();
        }
    }

    while (true) {
        system("cls");
        setColor(9);
        drawBox("User Menu");
        setColor(14);
        cout << "1. Take Test" << endl;
        cout << "2. View Results" << endl;
        cout << "3. Exit" << endl;
        setColor(7);
        cout << "Choose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            system("cls");
            setColor(10);
            drawBox("Take Test");
            setColor(7);
            currentUser.takeTest();
            _getch();
            break;
        case 2:
            system("cls");
            setColor(13);
            drawBox("View Results");
            setColor(7);
            currentUser.viewResults();
            _getch();
            break;
        case 0:
            system("cls");
            setColor(11);
            drawBox("Goodbye!");
            setColor(7);
            return 0;
        default:
            system("cls");
            setColor(12);
            drawBox("Error");
            setColor(7);
            cout << "Invalid choice. Try again." << endl;
            _getch();
        }
    }

    return 0;
}


void drawBox(const string& title) {
    cout << "+======================================+" << endl;
    cout << "|             " << title;
    for (size_t i = 0; i < 25 - title.size(); ++i) cout << " ";
    cout << "|" << endl;
    cout << "+--------------------------------------+" << endl;
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
