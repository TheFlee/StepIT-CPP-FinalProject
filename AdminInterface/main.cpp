#include "admin.h"

void setColor(int);
void drawBox(const string&);

int main() {
    Admin admin;
    int choice;
    loadTestsFromFile("../Include/tests.txt", allCategories);

    while (true) {
        if (!admin.isLoggedIn()) {
            system("cls");
            setColor(11);
            drawBox("Welcome");
            setColor(14);
            cout << "1. Login" << endl;
            cout << "0. Exit" << endl;
            setColor(7);
            cout << "Enter your choice: ";

            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: 
                system("cls");
                admin.loginAdmin();
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
                cout << "Invalid choice." << endl;
                _getch();
            }
        }
        else {
            system("cls");
            setColor(10);
            drawBox("Admin Menu");
            setColor(14);
            cout << "1. User Management" << endl;
            cout << "2. Test Management" << endl;
            cout << "3. View Test Results" << endl;
            cout << "4. System Tools" << endl;
            cout << "0. Exit" << endl;
            setColor(7);
            cout << "Enter your choice: ";

            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: {
                int userChoice;
                system("cls");
                setColor(9);
                drawBox("User Management");
                setColor(14);
                cout << "1. Create User" << endl;
                cout << "2. Edit User" << endl;
                cout << "3. Delete User" << endl;
                cout << "0. Back to Main Menu" << endl;
                setColor(7);
                cout << "Enter your choice: ";

                cin >> userChoice;

                switch (userChoice) {
                case 1: 
                    system("cls");
                    admin.createUser(); 
                    _getch();
                    break;
                case 2: 
                    system("cls");
                    admin.editUser(); 
                    _getch();
                    break;
                case 3: 
                    system("cls");
                    admin.deleteUser(); 
                    _getch();
                    break;
                case 0: 
                    break;
                default: 
                    system("cls"); 
                    cout << "Invalid choice." << endl;
                    _getch();
                }
                break;
            }
            case 2: {
                int testChoice;
                system("cls");
                setColor(13);
                drawBox("Test Management");
                setColor(14);
                cout << "1. Add Category" << endl;
                cout << "2. Add Test" << endl;
                cout << "3. Add Question" << endl;
                cout << "4. Import Tests from File" << endl;
                cout << "5. Export Tests to File" << endl;
                cout << "0. Back to Main Menu" << endl;
                setColor(7);
                cout << "Enter your choice: ";

                cin >> testChoice;
                cin.ignore();

                switch (testChoice) {
                case 1: 
                    system("cls");
                    admin.addCategory(); 
                    _getch();
                    break;
                case 2: 
                    system("cls");
                    admin.addTest(); 
                    _getch();
                    break;
                case 3: 
                    system("cls");
                    admin.addQuestion(); 
                    _getch();
                    break;
                case 4: 
                    system("cls");
                    admin.importTests(); 
                    _getch();
                    break;
                case 5: 
                    system("cls");
                    admin.exportTests(); 
                    _getch();
                    break;
                case 0: 
                    break;
                default: 
                    system("cls");
                    cout << "Invalid choice." << endl;
                    _getch();
                }
                break;
            }
            case 3: {
                int resultChoice;
                system("cls");
                setColor(6);
                drawBox("View Results");
                setColor(14);
                cout << "1. View Results by User" << endl;
                cout << "2. View Results by Category" << endl;
                cout << "3. View Results by Test" << endl;
                cout << "0. Back to Main Menu" << endl;
                setColor(7);
                cout << "Enter your choice: ";

                cin >> resultChoice;
                cin.ignore();

                switch (resultChoice) {
                case 1: 
                    system("cls");
                    admin.viewResultsByUser(); 
                    _getch();
                    break;
                case 2: 
                    system("cls");
                    admin.viewResultsByCategory(); 
                    _getch();
                    break;
                case 3: 
                    system("cls");
                    admin.viewResultsByTest(); 
                    _getch();
                    break;
                case 0: 
                    break;
                default: 
                    system("cls");
                    cout << "Invalid choice." << endl;
                    _getch();
                }
                break;
            }
            case 4: {
                int toolChoice;
                system("cls");
                setColor(12);
                drawBox("System Tools");
                setColor(14);
                cout << "1. Export All Statistics" << endl;
                cout << "2. Change Admin Credentials" << endl;
                cout << "0. Back to Main Menu" << endl;
                setColor(7);
                cout << "Enter your choice: ";
                cin >> toolChoice;
                cin.ignore();

                switch (toolChoice) {
                case 1: 
                    system("cls");
                    admin.exportStats(); 
                    _getch();
                    break;
                case 2: 
                    system("cls");
                    admin.changeCredentials();
                    _getch();
                    break;
                case 0: 
                    break;
                default: 
                    system("cls");
                    cout << "Invalid choice." << endl;
                    _getch();
                }
                break;
            }
            case 0:
                system("cls");
                setColor(11);
                drawBox("Goodbye!");
                setColor(7);
                return 0;
                break;
            default:
                system("cls");
                cout << "Invalid choice." << endl;
                _getch();
            }
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