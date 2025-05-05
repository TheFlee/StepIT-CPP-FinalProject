#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "conio.h"


using namespace std;

struct Answer {
    string text;
    bool isCorrect;
};

struct Question {
    string text;
    vector<Answer> answers;
};

struct Test {
    string name;
    vector<Question> questions;
};

struct Category {
    string name;
    vector<Test> tests;
};

struct TestResult {
    string testName;
    vector<string> attempts;
};

struct CategoryResult {
    string categoryName;
    vector<TestResult> tests;
};

void loadTestsFromFile(const string& filename, vector<Category>& categories);