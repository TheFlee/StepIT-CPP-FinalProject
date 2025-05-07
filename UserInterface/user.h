#pragma once
#include "../Include/Tests.h"
extern vector<Category> allCategories;

class User
{
private:
	string fullName;
	string address;
	string phoneNumber;
	string login;
	string password;
	bool loggedIn = false;

public:
	User(string fullName = "_", string address = "_", string phoneNumber = "_", string login = "_", string password = "_");

	string getFullName() const { return fullName; }
	bool isLoggedIn() const { return loggedIn; }

	void registerUser();
	void loginUser();
	void viewResults();
	void takeTest();

	void saveProgress(const string& category, const string& test, int questionIndex, int score);
	bool loadProgress(string& category, string& test, int& questionIndex, int& score);
	void deleteProgress();


};

