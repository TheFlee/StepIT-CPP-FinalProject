#pragma once
#include "../UserInterface/user.h"

class Admin
{
private:
	string login;
	string password;
	bool loggedIn = false;

public:
	Admin(string login = "_", string password = "_");

	bool isLoggedIn() const { return loggedIn; }

	void loginAdmin();
	void changeCredentials();

	void createUser();
	void deleteUser();
	void editUser();

	void viewResultsByCategory();
	void viewResultsByTest();
	void viewResultsByUser();
	void exportStats();

	void addCategory();
	void addTest();
	void addQuestion();
	void importTests();
	void exportTests();

};
