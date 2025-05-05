#include "user.h"

int main() {
    User currentUser;
    bool isAuthenticated = false;
    loadTestsFromFile("../Include/tests.txt", allCategories);

    while (!isAuthenticated) {
        system("cls");
        cout << "=== Welcome ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "Choose an option (1 or 2): ";

        int choice;
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
        case 1: 
            system("cls");
            currentUser.registerUser();
            cout << "Now please log in.\n";
            _getch();
            break;
        case 2:
            system("cls");
            currentUser.loginUser();
            _getch();
            if (currentUser.isLoggedIn()) isAuthenticated = true;
            break;
        default: 
            system("cls");
            cout << "Invalid choice. Please select 1 or 2.\n";
            _getch();
        }
    }

    while (true) {
        system("cls");
        cout << "=== Main Menu ===\n";
        cout << "1. Take Test\n";
        cout << "2. View Results\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            system("cls");
            currentUser.takeTest();
            _getch();
            break;
        case 2:
            system("cls");
            currentUser.viewResults();
            _getch();
            break;
        case 3:
            system("cls");
            cout << "Goodbye!\n";
            return 0;
        default:
            system("cls");
            cout << "Invalid choice. Try again.\n";
            _getch();
        }
    }

    return 0;
}
