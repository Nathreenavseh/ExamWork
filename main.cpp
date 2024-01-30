#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <map>
#include <filesystem>
#include <stdio.h>
#include <sstream>
using namespace std;
namespace fs = std::filesystem;

enum class UserRole {
    Admin,
    User
};
class Encryption {
public:
    static string shiftEncrypt(const string& input ) {
        string result = input;
        int shiftAmount = 3;
        for (size_t i = 0; i < input.size(); ++i) {
            char currentChar = input[i];

            if (isalpha(currentChar)) {
                char base = isupper(currentChar) ? 'A' : 'a';
                result[i] = static_cast<char>((currentChar - base + shiftAmount + 26) % 26 + base);
            }
            else if (isdigit(currentChar)) {
                result[i] = static_cast<char>((currentChar - '0' + shiftAmount + 10) % 10 + '0');
            }
        }

        return result;
    }
    static string shiftDecrypt(const string& input) {
        string result = input;
        int shiftAmount = -3;
        for (size_t i = 0; i < input.size(); ++i) {
            char currentChar = input[i];

            if (isalpha(currentChar)) {
                char base = isupper(currentChar) ? 'A' : 'a';
                result[i] = static_cast<char>((currentChar - base + shiftAmount + 26) % 26 + base);
            }
            else if (isdigit(currentChar)) {
                result[i] = static_cast<char>((currentChar - '0' + shiftAmount + 10) % 10 + '0');
            }
        }

        return result;
    }
};
class User {
private:
    string username, password, fullName, address, phone, userType;
    int unicalId;
public:
    User(){}
    User(int unicalId, string username, string password, string fullName, string address, string phone, string userType)
            : unicalId(unicalId), username(move(username)), password(move(password)),
              fullName(move(fullName)), address(move(address)),
              phone(move(phone)), userType(move(userType)) {
        this->unicalId = unicalId;

    }
    int getUnicalId() {return unicalId;}
    string getUsername() { return username; }
    string getPassword() { return password; }
    string getFullName() { return fullName; }
    string getAddress() { return address; }
    string getPhone() { return phone; }
    string getUsertype() { return userType; }

    void setUnicalId(int unicalId) {this->unicalId = unicalId;}
    void setUsername(string username) { this->username = move(username); }
    void setPassword(string password) { this->password = move(password); }
    void setFullName(string fullName) { this->fullName = move(fullName); }
    void setAddress(string  address) { this->address = move(address); }
    void setPhone(string phone) { this->phone = move(phone); }
    void setUsertype(string userType) { this->userType = move(userType); }
    friend ostream& operator<<(ostream& out, User& user);

};
ostream& operator<<(ostream& out, User& user) {
    out <<user.getUnicalId()<<endl<< Encryption::shiftEncrypt(user.getUsername()) << endl << Encryption::shiftEncrypt(user.getPassword()) << endl << Encryption::shiftEncrypt(user.getFullName()) << endl << Encryption::shiftEncrypt(user.getAddress()) << endl << Encryption::shiftEncrypt(user.getPhone()) << endl << user.getUsertype() << endl;
    return out;
}
class UsersVector {
private:
    bool AdminExist = false;
    vector<User> users;
public:
    bool isUserExist(string username) {
        for (auto& user : users) {
            if (user.getUsername() == username) {
                return true;
            }
        }
        return false;
    }
    bool isUserExistPasswordCorrect(const string& username, const string& password) {
        for (auto& user : users)
            if (user.getUsername() == username && user.getPassword() == password) {
                return true;
            }
        return false;
    }
    bool findAdmin() {
        for (auto& user : users) {
            if (user.getUsertype() == "admin") {
                return true;
            }
        }
        return false;
    }
    void addUsers(User&& user) {
        users.push_back(user);
    }
    void registrationAsAdminFromParametrs(int unicalId, const string& username, const string& password) {
        if (!findAdmin()) {
            if (!isUserExist(username)) {
                string userType = "admin";
                users.emplace_back(unicalId, username, password, "-", "-", "-", userType);
            }
        }
    }
    void registrationAsStudentFromParametrs(int unicalId, const string& username, const string& password, const string& fullName, const string& address, const string& phone) {
        if (!isUserExist(username)) {
            string userType = "student";
            users.emplace_back(unicalId, username, password, fullName, address, phone, userType);
        }
    }
    bool isUserIsAdmin(const string& username) {
        for (auto& user : users) {
            if (user.getUsertype() == "admin" && user.getUsername() == username) {
                return true;
            }
            return false;
        }
        return false;
    }
    vector<User>& getUsers() {
        return users;
    }
    int getUnicalId(const string& username, const string& password) {
        for (auto& user : users)
            if (user.getUsername() == username && user.getPassword() == password) {
                return user.getUnicalId();
            }
        return false;
    }
};
class LogIn {
private:
    bool logInAsAdmin = false;
    bool logInAsStudent = false;
public:
    void loginInSystem(UsersVector& usersWork, const string& username, const  string& password) {
        if (!usersWork.isUserExist(username)) {
            cerr << "Incorrect username. Please try again." << endl;
        }
        else {
            if (usersWork.isUserExist(username) && !usersWork.isUserExistPasswordCorrect(username, password)) {
                cerr << "Incorrect password. Please try again." << endl;
            }
            else {
                if (usersWork.isUserIsAdmin(username)) {
                    this->logInAsAdmin = true;
                    this->logInAsStudent = false;
                }
                else if (!usersWork.isUserIsAdmin(username)) {
                    this->logInAsAdmin = false;
                    this->logInAsStudent = true;
                }
                cout << "Login successful." << endl;
            }
        }
    }
    bool getLogInAsAdmin() { return logInAsAdmin; }
    bool getLogInAsStudent() { return logInAsStudent; }

};
class Question {
private:
    string text;
    map<int, string> options;
    int correctAnswer;

public:
    Question() {}
    Question(const string& qText) : text(qText){}
    void addOption(int option, const string& optionText) {
        this->options[option] = optionText;
    }
    void setCorrectAnswer(int correct) {
        this->correctAnswer = correct;
    }
    void setText(const string& qText) {
        this->text = qText;
    }
    string& getText(){ return text; }
    const map<int, string>& getOptions() { return options; }
    int getCorrectAnswer() const { return correctAnswer; }
    void setOptions(const map<int, string>& newOptions) {
        options = newOptions;
    }
    void setOption(int option, const string& optionText) {
        options[option] = optionText;
    }
    void setCorrectAnswerChar(int correct) {
        correctAnswer = correct;
    }

};
class Test {
private:
    string testName;
    vector<Question*> questions;
public:
    Test() {}
    Test(const string& tname) : testName(tname) {}
    void addQuestion(Question* question) {
        questions.push_back(question);
    }
    const vector<Question*>& getQuestions() const { return questions; }
    const string& getTestName() const { return testName; }
    void setQuestions(const vector<Question*>& newQuestions) {
        questions = newQuestions;
    }
};
class TestCategory {
private:
    string categoryName;
    vector<Test*> tests;
public:
    TestCategory() {}
    TestCategory(const string& name) : categoryName(name) {}
    void addTest(Test* test) {
        tests.push_back(test);
    }
    vector<Test*>& getTests() { return tests; }
    const string& getCategoryName() const { return categoryName; }
};
class TestSession {
private:
    vector<TestCategory*> categories;
    int totalQuestions;
public:
    TestSession() {}
    void addTestCategory(const string& categoryName) {
        for (TestCategory *category:  categories) {
            if(categoryName == category->getCategoryName()){
                return;
            }
        }
        categories.push_back(new TestCategory(categoryName));
    }
    void addTestToCategory(const string& categoryName, Test* test) {
        for (TestCategory *category: categories) {
            if (category->getCategoryName() == categoryName) {
                category->addTest(test);
                return;
            }
        }
    }
    void addQuestionToTest(Question* question, Test* test) {
        test->addQuestion(question);
    }
    const vector<TestCategory*>& getCategories() {
        return categories;
    }
    const vector<TestCategory*> getCategoriesByIndex(int index) {
        if (index >= 0 && index < categories.size()) {
            vector<TestCategory*> result;
            result.push_back(categories[index]);
            return result;
        }
        else {
            cerr << "Error: Index out of bounds." << endl;
            return vector<TestCategory*>();
        }
    }
    void setCategories(const vector<TestCategory*>& newCategories) {
        categories = newCategories;
    }
    void setTotalAmountOfQuestions(const string& categoryName, const string& testName){
        int counter = 0;
        for (TestCategory* category : categories) {
            if (category->getCategoryName() == categoryName) {
                for (Test *test: category->getTests()) {
                    if (test->getTestName() == testName) {
                        const vector<Test *> testsInCategory = category[0].getTests();
                        for (Test *findTest: testsInCategory) {
                            const vector<Question *> questionsInTest = test->getQuestions();
                            for (Question *question: questionsInTest){
                                counter++;
                            }
                            break;
                        }
                    }
                }
            }
        }
        this->totalQuestions = counter;
    };
    int getTotalAmountOfQuestions() {
        return totalQuestions;
    }
};
class workWithFiles {
public:
    void saveUsersToFile(UsersVector& usersWork, const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error: Could not open the file for writing." << endl;
            return;
        }
        for (auto& user : usersWork.getUsers()) {
            outFile << user;
        }
        outFile.close();
    }
    void loadUsersFromFile(UsersVector& usersWork, const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error: Could not open the file for reading." << endl;
            return;
        }
        while (true) {
            string unicalId, username, password, fullName, address, phone, userType;
            getline(inFile, unicalId);
            inFile.clear();
            getline(inFile, username);
            inFile.clear();
            getline(inFile, password);
            inFile.clear();
            getline(inFile, fullName);
            inFile.clear();
            getline(inFile, address);
            inFile.clear();
            getline(inFile, phone);
            inFile.clear();
            getline(inFile, userType);
            inFile.clear();

            int value;
            istringstream iss(unicalId);
            iss >> value;
            if(userType == ""){
                inFile.close();
                return;
            }
            usersWork.addUsers(User (move(value), move(Encryption::shiftDecrypt(username)), move(Encryption::shiftDecrypt(password)),move(Encryption::shiftDecrypt(fullName)),move(Encryption::shiftDecrypt(address)), move(Encryption::shiftDecrypt(phone)), move(userType )));

        }
    }
    void saveTests(TestSession& testSessions, const string& filename) {
        string categoryName, testName, questions;
        int counterCategories = 0;
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error: Could not open the file for writing." << endl;
            return;
        }
        for (TestCategory* category : testSessions.getCategories()) {
            const vector<TestCategory*> categories = testSessions.getCategoriesByIndex(counterCategories);
            if (!categories.empty()) {
                categoryName = categories[0]->getCategoryName();
                const vector<Test*> testsInCategory = categories[0]->getTests();
                for (Test* findTest : testsInCategory) {
                    testName = findTest->getTestName();
                    const vector<Question*> questionsInTest = findTest->getQuestions();
                    for (Question* question : questionsInTest) {
                        const map<int, string>& options = question->getOptions();
                        outFile << categoryName<<endl<<testName<<endl<<question->getText() << endl;
                        for (const auto& option : options) {
                            outFile<<option.second<<endl;
                        }
                        outFile<<question->getCorrectAnswer()<<endl;

                    }

                }
            }
            counterCategories++;
        }
        outFile.close();
    }
    void loadTests(TestSession& testSessions, const string& filename){
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error: Could not open the file for reading." << endl;
            return;
        }
        while (!inFile.eof()) {
            string categoryName, testName, questionText, optionText1, optionText2, optionText3, optionText4;
            int correctAnswer;

            getline(inFile, categoryName);
            getline(inFile, testName);
            getline(inFile, questionText);
            getline(inFile, optionText1);
            getline(inFile, optionText2);
            getline(inFile, optionText3);
            getline(inFile, optionText4);
            inFile >> correctAnswer;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
            if(categoryName != "" && testName != "" && questionText != "" && optionText1 != "" && optionText2 != "" && optionText3 != "" && optionText4 != "" &&  correctAnswer != 0){
                bool addInExistCategory = true;
                bool addInExistTest = true;
                int counter = 0;
                for (TestCategory *category: testSessions.getCategories()) {
                    if(categoryName == category->getCategoryName()){
                        addInExistCategory = false;
                        const vector<TestCategory *> categories = testSessions.getCategoriesByIndex(counter);
                        const vector<Test *> testsInCategory = categories[0]->getTests();
                        int counterTest = 0;
                        for (Test *findTest : testsInCategory) {
                            if(testName == findTest->getTestName()){
                                addInExistTest = false;
                                Question* question = new Question(questionText);
                                question->setText(move(questionText));
                                question->addOption(1, move(optionText1));
                                question->addOption(2, move(optionText2));
                                question->addOption(3, move(optionText3));
                                question->addOption(4, move(optionText4));
                                question->setCorrectAnswer(move(correctAnswer));
                                testSessions.addQuestionToTest(move(question), move(findTest));
                            }
                            counterTest++;
                        }
                        if(addInExistTest){
                            Test* test = new Test(testName);
                            testSessions.addTestToCategory(categoryName, test);
                            Question* question = new Question(questionText);
                            question->setText(move(questionText));
                            question->addOption(1, move(optionText1));
                            question->addOption(2, move(optionText2));
                            question->addOption(3, move(optionText3));
                            question->addOption(4, move(optionText4));
                            question->setCorrectAnswer(move(correctAnswer));
                            testSessions.addQuestionToTest(move(question), move(test));
                        }
                    }
                    counter++;
                }
                if(addInExistCategory && addInExistTest){
                    testSessions.addTestCategory(categoryName);
                    Test* test = new Test(testName);
                    testSessions.addTestToCategory(categoryName, test);
                    Question* question = new Question(questionText);
                    question->addOption(1, optionText1);
                    question->addOption(2, optionText2);
                    question->addOption(3, optionText3);
                    question->addOption(4, optionText4);
                    question->setCorrectAnswer(correctAnswer);
                    testSessions.addQuestionToTest(question, test);
                }
                correctAnswer = 0;
            }
        }
        inFile.close();
    }
    void addResultsToTestResults(const string& filename, const string& category, const string& test,const string& question, int score){
        ofstream outFile(filename, ios::app);
        outFile <<category<<endl<<test<<endl<<question<<endl<<score<<endl;
        outFile.close();
    }
    int loadTestForResult(const string& filename, const string& category, const string& test){
        ifstream inFile(filename);
        int correctAnswer = 0;
        if (inFile) {
            string none, none1, none2, none3;
            while (!inFile.eof()) {
                int checkCorrectAnswer = 0;
                getline(inFile, none);
                inFile.clear();
                getline(inFile, none1);
                inFile.clear();
                getline(inFile, none2);
                inFile.clear();
                getline(inFile, none3);
                if(none3 =="1" ){
                    correctAnswer++;
                }
                inFile.clear();
                if(none3 == ""){
                    break;
                }
            }
        }
        inFile.close();
        return correctAnswer;
    }
    void removeTest(const string& filename){
        ifstream inFile(filename);
        remove(filename.c_str());
    }
    string loadLastQuestionFromPausedTest(const string& filename){
        ifstream inFile(filename);
        string nothing;
        string lastQuestion;
        string lastQuestioncheck;
        while (true) {
            getline(inFile, nothing);
            inFile.clear();
            getline(inFile, nothing);
            inFile.clear();
            getline(inFile, lastQuestioncheck);
            inFile.clear();
            if(lastQuestioncheck!=""){
                lastQuestion = lastQuestioncheck;
            }
            else{
                break;
            }
            getline(inFile, nothing);
            inFile.clear();
        }
        inFile.close();
        return lastQuestion;
    }
    vector<string> getTestFilesInFolder(const string& folderPath) {
        vector<string> testFiles;
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                testFiles.push_back(entry.path().filename().string());
            }
        }
        return testFiles;
    }
    int getUnicalId(const string& filename){
        ifstream inFile(filename);
        int unicalId = 0;
        if (inFile) {
            string none;
            string UnicalIdcheck;
            while (!inFile.eof()) {
                int checkCorrectAnswer = 0;
                getline(inFile, UnicalIdcheck);
                inFile.clear();
                int value;
                istringstream iss(UnicalIdcheck);
                iss >> value;
                if(unicalId<=value && UnicalIdcheck != ""){
                    unicalId=value+1;
                }
                else if(UnicalIdcheck == ""){
                    inFile.close();
                    return unicalId;
                }
                getline(inFile, none);
                inFile.clear();
                getline(inFile, none);
                inFile.clear();
                getline(inFile, none);
                inFile.clear();
                getline(inFile, none);
                inFile.clear();
                getline(inFile, none);
                inFile.clear();
                getline(inFile, none);
                inFile.clear();
            }
        }
        return 0;
    }

};
class Checker{
public:
    static int selectAnswers(int from, int till);
};
int Checker::selectAnswers(int from, int till){
    int selectAnswer;
    stringstream ss;
    stringstream sss;
    ss<<from;
    sss<<till;
    string error = "Enter index from " + ss.str()+ " to " + sss.str()+ ". Try again: ";
    while (true) {
        cin >> selectAnswer;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
//                throw out_of_range(error);
            cout << "Invalid input. Please enter a valid number: ";
        }
        else if (selectAnswer == from || (selectAnswer > from-1 && selectAnswer <till+1 )) {
            cin.ignore();
            return selectAnswer;
        }
        else {
//                throw out_of_range(error);
            cout << "Invalid input. Please enter a valid number: " ;
        }
    }
}
class Administration  {
private:
    workWithFiles files;

public:
    void showResultOfUser(TestSession& testsissions, UsersVector& usersWork, string pathToFolder, int unicalId){
        int counter = 0;
        string unicalIdString = to_string(unicalId);
        for (TestCategory* category : testsissions.getCategories()) {
            for (Test* test : category->getTests()) {
                const vector<Question *> questionsInTest = test->getQuestions();
                testsissions.setTotalAmountOfQuestions(category->getCategoryName(), test->getTestName());
                vector<string> testFiles = files.getTestFilesInFolder(pathToFolder);
                string fileName = "Results/"+category->getCategoryName()  + "_" + test->getTestName() + "_" +"unicalId_" + unicalIdString + ".txt";
                string fileNameForCheck = category->getCategoryName()  + "_" + test->getTestName() + "_" +"unicalId_" + unicalIdString + ".txt";
                for (const auto& fileNames : testFiles) {
                    if(fileNameForCheck == fileNames) {
                        int counter1 = 1;
                        for (Question *question: questionsInTest) {
                            if(files.loadLastQuestionFromPausedTest(fileName) == question->getText()){
                                break;
                            }
                            counter1++;
                        }
                        int correct = files.loadTestForResult(fileName, category->getCategoryName(), test->getTestName());
                        cout<<"Category: "<<category->getCategoryName()<<endl<<"Test: "<<test->getTestName()<<endl;
                        if(counter1 == testsissions.getTotalAmountOfQuestions()){
                            cout<<"User answered "<<correct<<" from "<<testsissions.getTotalAmountOfQuestions()<<endl;
                            int percentage = correct*100 / testsissions.getTotalAmountOfQuestions();
                            cout<<"User score is "<< percentage<<"%"<<endl<<endl;
                        }
                        else{
                            cout<<"User test is paused. User are stoped at "<<counter1<<" question";
                            cout<<endl<<"Till now user answered "<<correct<<" from "<<testsissions.getTotalAmountOfQuestions()<<endl;
                            int percentage = correct*100 / testsissions.getTotalAmountOfQuestions();
                            cout<<"User score is "<< percentage<<"%"<<endl<<endl;
                        }
                        counter++;
                    }
                }
            }
        }
        if(counter == 0){
            cout<<endl<<"This user didn't pass any exams "<<endl<<endl;
        }
    }
    void showResultsByTests(TestSession& testsissions, UsersVector& usersWork, string& pathToFolder, const string& categoryName, const string& testName){
        int counter = 0;
        vector<User> users = usersWork.getUsers();
        for(auto& user : users){
            if(user.getUsertype() != "admin"){
                string unicalIdString = to_string(user.getUnicalId());
                for (TestCategory* category : testsissions.getCategories()) {
                    for (Test* test : category->getTests()) {
                        if(category->getCategoryName() == categoryName && test->getTestName() == testName) {
                            const vector<Question *> questionsInTest = test->getQuestions();
                            testsissions.setTotalAmountOfQuestions(category->getCategoryName(), test->getTestName());
                            vector<string> testFiles = files.getTestFilesInFolder(pathToFolder);
                            string fileName =
                                    "Results/" + category->getCategoryName() + "_" + test->getTestName() + "_" +
                                    "unicalId_" + unicalIdString + ".txt";
                            string fileNameForCheck =
                                    category->getCategoryName() + "_" + test->getTestName() + "_" + "unicalId_" +
                                    unicalIdString + ".txt";
                            for (const auto &fileNames: testFiles) {
                                if (fileNameForCheck == fileNames) {

                                    int counter1 = 1;
                                    for (Question *question: questionsInTest) {
                                        if (files.loadLastQuestionFromPausedTest(fileName) == question->getText()) {
                                            break;
                                        }
                                        counter1++;
                                    }
                                    int correct = files.loadTestForResult(fileName, category->getCategoryName(),
                                                                          test->getTestName());
                                    cout<<"User "<<user.getFullName()<<endl;
                                    cout << "Category: " << category->getCategoryName() << endl << "Test: "
                                         << test->getTestName() << endl;
                                    if (counter1 == testsissions.getTotalAmountOfQuestions()) {
                                        cout << "User answered " << correct << " from "<< testsissions.getTotalAmountOfQuestions() << endl;
                                        int percentage = correct * 100 / testsissions.getTotalAmountOfQuestions();
                                        cout << "User score is " << percentage << "%" << endl << endl;
                                    } else {
                                        cout << "User test is paused. User are stoped at " << counter1 << " question";
                                        cout << endl << "Till now user answered " << correct << " from "
                                             << testsissions.getTotalAmountOfQuestions() << endl;
                                        int percentage = correct * 100 / testsissions.getTotalAmountOfQuestions();
                                        cout << "User score is " << percentage << "%" << endl << endl;
                                    }
                                    counter++;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(counter == 0){
            cout<<endl<<"Тo one has taken this test yet "<<endl<<endl;
        }
    }
    void showResultByCategory(TestSession& testsissions, UsersVector& usersWork, string& pathToFolder, const string& categoryName){
        int counter = 0;
        vector<User> users = usersWork.getUsers();
        for(auto& user : users){
            if(user.getUsertype() != "admin"){
                string unicalIdString = to_string(user.getUnicalId());
                for (TestCategory* category : testsissions.getCategories()) {
                    if(category->getCategoryName() == categoryName) {
                        for (Test* test : category->getTests()) {
                            const vector<Question *> questionsInTest = test->getQuestions();
                            testsissions.setTotalAmountOfQuestions(category->getCategoryName(), test->getTestName());
                            vector<string> testFiles = files.getTestFilesInFolder(pathToFolder);
                            string fileName =
                                    "Results/" + category->getCategoryName() + "_" + test->getTestName() + "_" +
                                    "unicalId_" + unicalIdString + ".txt";
                            string fileNameForCheck =
                                    category->getCategoryName() + "_" + test->getTestName() + "_" + "unicalId_" +
                                    unicalIdString + ".txt";
                            for (const auto &fileNames: testFiles) {
                                if (fileNameForCheck == fileNames) {

                                    int counter1 = 1;
                                    for (Question *question: questionsInTest) {
                                        if (files.loadLastQuestionFromPausedTest(fileName) == question->getText()) {
                                            break;
                                        }
                                        counter1++;
                                    }
                                    int correct = files.loadTestForResult(fileName, category->getCategoryName(),test->getTestName());
                                    cout<<"User "<<user.getFullName()<<endl;
                                    cout << "Category: " << category->getCategoryName() << endl << "Test: " << test->getTestName() << endl;
                                    if (counter1 == testsissions.getTotalAmountOfQuestions()) {
                                        cout << "User answered " << correct << " from "
                                             << testsissions.getTotalAmountOfQuestions() << endl;
                                        int percentage = correct * 100 / testsissions.getTotalAmountOfQuestions();
                                        cout << "User score is " << percentage << "%" << endl << endl;
                                    } else {
                                        cout << "User test is paused. User are stoped at " << counter1 << " question";
                                        cout << endl << "Till now user answered " << correct << " from "
                                             << testsissions.getTotalAmountOfQuestions() << endl;
                                        int percentage = correct * 100 / testsissions.getTotalAmountOfQuestions();
                                        cout << "User score is " << percentage << "%" << endl << endl;
                                    }
                                    counter++;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(counter == 0){
            cout<<endl<<"Тo one has taken this test yet "<<endl<<endl;
        }
    }
    void showInfoAboutUser(UsersVector& usersWork, int indexToDisplay){
        vector<User> users = usersWork.getUsers();
        size_t select = indexToDisplay;
        if (select <= users.size()) {
            cout << "===== User info =====\n";
            cout << "Unical id: " <<users[indexToDisplay].getUnicalId()<<endl;
            cout << "Username: " <<users[indexToDisplay].getUsername()<<endl;
            cout << "Password: "<<users[indexToDisplay].getPassword()<<endl;
            cout << "Fullname: "<<users[indexToDisplay].getFullName()<<endl;
            cout << "Adres: "<<users[indexToDisplay].getAddress()<<endl;
            cout << "Phone: "<<users[indexToDisplay].getPhone()<<endl;
        }
    }

    void setNewFullnameToUser(UsersVector& usersWork, string newFullname, int indexVector){
        size_t indexToDisplay = indexVector;
        vector<User>& users = usersWork.getUsers();
        if (indexToDisplay <= users.size()) {
            users[indexToDisplay].setFullName(newFullname);
        }
    }
    void setNewUsernameToUser(UsersVector& usersWork, string newUsername, int indexVector){
        size_t indexToDisplay = indexVector;
        vector<User>& users = usersWork.getUsers();
        if (indexToDisplay <= users.size()) {
            users[indexToDisplay].setUsername(newUsername);
        }
    }
    void setNewPhoneNumberToUser(UsersVector& usersWork, string phonenumber, int indexVector){
        size_t indexToDisplay = indexVector;
        vector<User>& users = usersWork.getUsers();
        if (indexToDisplay <= users.size()) {
            users[indexToDisplay].setPhone(phonenumber);
        }
    }
    void setNewAddressToUser(UsersVector& usersWork, string address, int indexVector){
        size_t indexToDisplay = indexVector;
        vector<User>& users = usersWork.getUsers();
        if (indexToDisplay <= users.size()) {
            users[indexToDisplay].setAddress(address);
        }
    }
    void setNewPasswordToUser(UsersVector& usersWork, string password, int indexVector){
        size_t indexToDisplay = indexVector;
        vector<User>& users = usersWork.getUsers();
        if (indexToDisplay <= users.size()) {
            users[indexToDisplay].setPassword(password);
        }
    }
    void showUsersList(UsersVector& usersWork){
        vector<User> users = usersWork.getUsers();
        int i = 0;
        for(auto& user : users){
            if(user.getUsertype() != "admin"){
                cout<<i+1<<"." <<user.getFullName()<<endl;
                i++;
            }
        }
    }
    void deleteUser(UsersVector& usersWork, size_t indexVector){
        vector<User>& users = usersWork.getUsers();
        if (indexVector < users.size()) {
            users.erase(users.begin() + indexVector);
        }
    }
    int countOfUsers(UsersVector& usersWork){
        vector<User> users = usersWork.getUsers();
        int i = 0;
        for(auto& user : users){
            if(user.getUsertype() != "admin"){
                i++;
            }
        }
        return i;
    }
};
class Student{
    workWithFiles files;
public:
    void startTest(TestSession& testsissions, int unicalId, const string& categoryName, const string& testName, string pathToFolder, bool restartOrContinue) {
        int selectAnswer;
        int score;
        testsissions.setTotalAmountOfQuestions(categoryName,testName);
        string unicalIdString = to_string(unicalId);
        for (TestCategory* category : testsissions.getCategories()) {
            int coutCorrect = 0;
            if (category->getCategoryName() == categoryName) {
                for (Test* test : category->getTests()) {
                    if (test->getTestName() == testName) {
                        const vector<Question *> questionsInTest = test->getQuestions();
                        vector<string> testFiles = files.getTestFilesInFolder(pathToFolder);
                        string fileName = "Results/"+categoryName  + "_" + testName + "_" +"unicalId_" + unicalIdString + ".txt";
                        string fileNameForCheck = categoryName  + "_" + testName + "_" +"unicalId_" + unicalIdString + ".txt";
                        int choose = 0;
                        for (const auto& fileNames : testFiles) {
                            if(fileNameForCheck == fileNames){
                                int counter1 = 0;
                                for (Question *question: questionsInTest) {
                                    if(files.loadLastQuestionFromPausedTest(fileName) == question->getText()){
                                        break;
                                    }
                                    counter1++;
                                }
                                if(!restartOrContinue){
                                    files.removeTest(fileName);
                                }
                                if(restartOrContinue){
                                    int counter2 = 0;
                                    for (Question *question: questionsInTest) {
                                        if(counter1 < counter2){
                                            cout << "Question: " << question->getText() << endl;
                                            const map<int, string> &options = question->getOptions();
                                            for (const auto &option: options) {
                                                cout << "Option " << option.first << ": " << option.second << endl;
                                            }
                                            cout << "Enter your answer or press 0 to exit test: ";
                                            selectAnswer = Checker::selectAnswers(0,4);
                                            if(question->getCorrectAnswer() == selectAnswer) {
                                                cout<<"Correct! "<<endl;
                                                files.addResultsToTestResults(fileName,categoryName,testName, question->getText(),1);
                                                coutCorrect++;
                                            }
                                            else if(selectAnswer == 0){
                                                return;
                                            }
                                            else{
                                                cout<<"Incorrect! "<<endl;
                                                files.addResultsToTestResults(fileName,categoryName,testName, question->getText(),0);
                                            }
                                        }
                                        counter2++;
                                    }
                                    int correct = files.loadTestForResult(fileName, categoryName, testName);
                                    coutCorrect+=correct;

                                    cout<<endl<<"You answered "<<coutCorrect<<" from "<<testsissions.getTotalAmountOfQuestions()<<endl;
                                    int percentage = coutCorrect*100 / testsissions.getTotalAmountOfQuestions();
                                    cout<<"Your score is "<< percentage<<"%"<<endl;
                                    return;
                                }
                            }
                        }
                        int correct = 0;
                        for (Question *question: questionsInTest) {
                            cout << "Question: " << question->getText() << endl;
                            const map<int, string> &options = question->getOptions();
                            for (const auto &option: options) {
                                cout << "Option " << option.first << ": " << option.second << endl;
                            }
                            cout << "Enter your answer or press 0 to exit test: ";
                            selectAnswer = Checker::selectAnswers(0,4);

                            if (question->getCorrectAnswer() == selectAnswer) {
                                cout << "Correct! " << endl;
                                correct++;
                                files.addResultsToTestResults(fileName, categoryName, testName, question->getText(), 1);
                            }
                            else if (selectAnswer == 0) {
                                return;
                            }
                            else {
                                cout << "Incorrect! " << endl;
                                files.addResultsToTestResults(fileName, categoryName, testName, question->getText(), 0);
                            }
                        }
                        cout<<endl<<"You answer "<<correct<<" from "<<testsissions.getTotalAmountOfQuestions()<<endl<<endl;
                        int percentage = correct*100 / testsissions.getTotalAmountOfQuestions();
                        cout<<percentage<<"%"<<endl;
                    }
                }
                return;
            }
        }
    }
    void showResults(TestSession& testsissions, string pathToFolder, int unicalId){
        int counter = 0;
        string unicalIdString = to_string(unicalId);
        for (TestCategory* category : testsissions.getCategories()) {
            for (Test* test : category->getTests()) {
                const vector<Question *> questionsInTest = test->getQuestions();
                testsissions.setTotalAmountOfQuestions(category->getCategoryName(), test->getTestName());
                vector<string> testFiles = files.getTestFilesInFolder(pathToFolder);
                string fileName = "Results/"+category->getCategoryName()  + "_" + test->getTestName() + "_" +"unicalId_" + unicalIdString + ".txt";
                string fileNameForCheck = category->getCategoryName()  + "_" + test->getTestName() + "_" +"unicalId_" + unicalIdString + ".txt";
                for (const auto& fileNames : testFiles) {
                    if(fileNameForCheck == fileNames) {
                        int counter1 = 1;
                        for (Question *question: questionsInTest) {
                            if(files.loadLastQuestionFromPausedTest(fileName) == question->getText()){
                                break;
                            }
                            counter1++;
                        }
                        int correct = files.loadTestForResult(fileName, category->getCategoryName(), test->getTestName());
                        cout<<"Category: "<<category->getCategoryName()<<endl<<"Test: "<<test->getTestName()<<endl;
                        if(counter1 == testsissions.getTotalAmountOfQuestions()){
                            cout<<"You answered "<<correct<<" from "<<testsissions.getTotalAmountOfQuestions()<<endl;
                            int percentage = correct*100 / testsissions.getTotalAmountOfQuestions();
                            cout<<"Your score is "<< percentage<<"%"<<endl<<endl;
                        }
                        else{
                            cout<<"Your test is paused. You are stoped at "<<counter1<<" question";
                            cout<<endl<<"Till now you answered "<<correct<<" from "<<testsissions.getTotalAmountOfQuestions()<<endl;
                            int percentage = correct*100 / testsissions.getTotalAmountOfQuestions();
                            cout<<"Your score is "<< percentage<<"%"<<endl<<endl;
                        }
                        counter++;
                    }
                }
            }
        }
        if(counter == 0){
            cout<<endl<<"You dont pass any exams "<<endl<<endl;
        }
    }
};
class ConsoleInterface {
    string pathToFolder = "/Users/a2141pro4/CLionProjects/ExamWork/cmake-build-debug/Results";
    string usersTxt = "users.txt";
    string testsTxt = "alltests.txt";
    int unicalId;
    string username, password, fullName, address, phone;
    string categoryName, testName;
    Administration administrator;
    Student student;
    workWithFiles workWithFile;
    UsersVector usersWork;
    LogIn logForReg;
    TestSession session;

    //Main menu
    void displayMainMenu() {
        cout << "===== Main Menu =====\n";
        cout << "1. Sign in\n";
        cout << "2. Log in\n";
        cout << "3. Exit\n";
        cout << "=====================\n";
        cout << "Enter your choice: ";
    }
    void displaySignInMenu() {
        bool flag = false;
        while (!flag) {
            cout << "===== Sign In Menu =====\n";
            cout << "1. Admin Registration\n";
            cout << "2. Student Registration\n";
            cout << "3. Back to Main Menu\n";
            cout << "=========================\n";
            cout << "Enter your choice: ";
            int choice = Checker::selectAnswers(1,3);

            switch (choice) {
                case 1:
                    displayRegistrationAsAdmin();
                    break;
                case 2:
                    displayRegistrationAsStudent();
                    break;
                case 3:
                    flag = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    void displayLogInMenu() {
        cout << "===== Log In Menu =====\n";
        cout << "Enter username: ";
        getline(cin, username);
        while (true) {
            if (!usersWork.isUserExist(username)) {
                cout << "User with this username not found. Please enter correct username.\n";
                cout << "Enter your username: ";
                getline(cin, username);
            }
            else {
                break;
            }
        }
        cout << "Enter password: ";
        getline(cin, password);
        while (true) {
            if (!usersWork.isUserExistPasswordCorrect(username, password)) {
                cout << "Incorrect password. Please enter correct password.\n";
                cout << "Enter password: ";
                getline(cin, password);
            }
            else {
                break;
            }
        }
        logForReg.loginInSystem(usersWork, username, password);
        this->unicalId = usersWork.getUnicalId(username, password);
        if (logForReg.getLogInAsAdmin() == true && logForReg.getLogInAsStudent() == false) {
            displayIfLogInAsAdmin();
        }
        else if (logForReg.getLogInAsAdmin() == false && logForReg.getLogInAsStudent() == true) {
            displayIfLogInAsStudent();
        }
    }
    void mainTestsMenu() {
        bool flag = false;
        while (!flag) {
            cout << "===== Main test Menu =====\n";
            cout << "1. Add test\n";
            cout << "2. Edit information\n";
            cout << "3. Back to Main Menu\n";
            cout << "=========================\n";
            cout << "Enter your choice: ";
            int choice = Checker::selectAnswers(1,3);
            switch (choice) {
                case 1:
                    workWithAddingTest();
                    break;
                case 2:
                    editTests();
                    break;
                case 3:
                    flag = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    //Admin
    void displayRegistrationAsAdmin() {
        if (usersWork.findAdmin()) {
            cerr << "Admin is already registered. Cannot register another admin.\n";
        }
        if (!usersWork.findAdmin()) {
            cout << "===== Admin Registration =====\n";
            cout << "Enter username: ";
            getline(cin, username);
            while (true) {
                if (usersWork.isUserExist(username)) {
                    cout << "User with this username already exists. Please choose another username.\n";
                    cout << "Enter your username: ";
                    getline(cin, username);
                }
                else {
                    break;
                }
            }
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Admin registration successful.\n";
            usersWork.registrationAsAdminFromParametrs(move(workWithFile.getUnicalId(this->usersTxt)), move(username), move(password));
        }
    }
    void displayIfLogInAsAdmin() {
        if (logForReg.getLogInAsAdmin() == true && logForReg.getLogInAsStudent() == false) {
            bool flag = false;
            while (!flag) {
                cout << "===== Admin Menu =====\n";
                cout << "1. Tests menu\n";
                cout << "2. Show results\n";
                cout << "3. Show information about users\n";
                cout << "4. Change information about users\n";
                cout << "5. Back to Main Menu\n";
                cout << "=========================\n";
                cout << "Enter your choice: ";
                int choice = Checker::selectAnswers(1,5);
                switch (choice) {
                    case 1:
                        mainTestsMenu();
                        break;
                    case 2:
                        showResultsToAdmin();
                        break;
                    case 3:
                        showInfoAboutUsersToAdmin();
                        break;
                    case 4:
                        displayChangeInformation();
                        break;
                    case 5:
                        flag = true;
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
    void displayChangeInformation(){
        bool flag = false;
        while (!flag) {
            cout << "===== Change info of users menu =====\n";
            cout << "1. Change name\n";
            cout << "2. Change username\n";
            cout << "3. Change password\n";
            cout << "4. Change phone\n";
            cout << "5. Change address\n";
            cout << "6. Delete user\n";
            cout << "7. Back to Main Menu\n";
            cout << "=========================\n";
            cout << "Enter your choice: ";
            int choice = Checker::selectAnswers(1,7);
            switch (choice) {
                case 1:
                    changeFullnameUser();
                    break;
                case 2:
                    changeUsernameOfUser();
                    break;
                case 3:
                    changePasswordfUser();
                    break;
                case 4:
                    changePhonenumberOfUser();
                    break;
                case 5:
                    changeAddressofUser();
                case 6:
                    deleteUser();
                    break;
                case 7:
                    flag = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }


    }
    void showInfoAboutUsersToAdmin(){
        int unicalId;
        vector<User> users = usersWork.getUsers();
        int i = 0;
        for(auto& user : users){
            if(user.getUsertype() != "admin"){
                cout<<i+1<<"." <<user.getFullName()<<endl;
                i++;
            }
        }
        cout<<endl<<"Select user: ";
        int select = Checker::selectAnswers(1,i);
        administrator.showInfoAboutUser(usersWork, select);
    }
    int ShowingToMakeSelectUserToChangeInfo(){
        int counter = 0;
        administrator.showUsersList(usersWork);
        cout << "Enter your choice or press 0 to back to Main Menu: ";
        int indexVector = Checker::selectAnswers(0,administrator.countOfUsers(usersWork));
        if(indexVector == 0 ){return 0;}
        cout << "==============================         NOTICE         ==============================    \n";
        cout << "Once you have edited information, you will not be able to get the old information back. "<<endl;
        cout << "1. Continue changes\n";
        cout << "2. Back to Main Menu\n";
        cout << "Enter your choice: ";
        int selection = Checker::selectAnswers(1,2);
        if(selection == 2 ){return 0;}
        return indexVector;
    }
    void changeFullnameUser(){
        int indexVector = ShowingToMakeSelectUserToChangeInfo();
        if(indexVector == 0){return;}
        string newFullname;
        cout<<"Enter new fullname: ";
        getline(cin, newFullname);
        administrator.setNewFullnameToUser(usersWork, newFullname, indexVector);
        cout<<endl<<"User name changed successfully "<<endl<<endl;
    }
    void changeUsernameOfUser(){
        int indexVector = ShowingToMakeSelectUserToChangeInfo();
        if(indexVector == 0){return;}
        string newUserName;
        cout<<"Enter new username: ";
        getline(cin, newUserName);
        administrator.setNewUsernameToUser(usersWork, newUserName, indexVector);
        cout<<endl<<"User username changed successfully "<<endl<<endl;
    }
    void changePhonenumberOfUser(){
        int indexVector = ShowingToMakeSelectUserToChangeInfo();
        if(indexVector == 0){return;}
        string newPhobenumber;
        cout<<"Enter new phone number: ";
        getline(cin, newPhobenumber);
        administrator.setNewPhoneNumberToUser(usersWork, newPhobenumber, indexVector);
        cout<<endl<<"User phone number changed successfully "<<endl<<endl;
    }
    void changeAddressofUser(){
        int indexVector = ShowingToMakeSelectUserToChangeInfo();
        if(indexVector == 0){return;}
        string newAddress;
        cout<<"Enter new address: ";
        getline(cin,newAddress);
        administrator.setNewAddressToUser(usersWork, newAddress, indexVector);
        cout<<endl<<"User address changed successfully "<<endl<<endl;
    }
    void changePasswordfUser(){
        int indexVector = ShowingToMakeSelectUserToChangeInfo();
        if(indexVector == 0){return;}
        string newPassword;
        cout<<"Enter new address: ";
        getline(cin,newPassword);
        administrator.setNewPasswordToUser(usersWork, newPassword, indexVector);
        cout<<endl<<"User password changed successfully "<<endl<<endl;
    }
    void deleteUser(){
        int indexVector = ShowingToMakeSelectUserToChangeInfo();
        if(indexVector == 0){return;}
        administrator.deleteUser(usersWork, indexVector);
        cout<<"User deleted successfully "<<endl;
    }
    //Student
    void displayRegistrationAsStudent() {
        cout << "===== Student Registration =====\n";
        cout << "Enter username: ";
        getline(cin, username);
        while (true) {
            if (usersWork.isUserExist(username)) {
                cout << "User with this username already exists. Please choose another username.\n";
                cout << "Enter your username: ";
                getline(cin, username);
            }
            else {
                break;
            }
        }
        cout << "Enter password: ";
        getline(cin, password);
        cout << "Enter full name: ";
        getline(cin, fullName);
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter phone: ";
        getline(cin, phone);
        cout << "Student registration successful.\n";
        usersWork.registrationAsStudentFromParametrs(move(workWithFile.getUnicalId(this->usersTxt)), move(username), move(password), move(fullName), move(address), move(phone));
        cout << endl;
    }
    void displayIfLogInAsStudent() {
        if (logForReg.getLogInAsAdmin() == false && logForReg.getLogInAsStudent() && true) {
            bool flag = false;
            while (!flag) {
                cout << "===== Student Menu =====\n";
                cout << "1. Start Test\n";
                cout << "2. Show results\n";
                cout << "3. Back to Main Menu\n";
                cout << "=========================\n";
                cout << "Enter your choice: ";
                int choice = Checker::selectAnswers(1,3);
                switch (choice) {
                    case 1:
                        testingAsStudent();
                        break;
                    case 2:
                        student.showResults(session, this->pathToFolder, this->unicalId);
                        break;
                    case 3:
                        flag = true;
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
    void testingAsStudent() {
        cout << "===== Choose test Menu =====\n";
        bool restartOrContinue = true;
        int counter = 0;
        int choice;
        string categoryName, testName;
        for (TestCategory *category: session.getCategories()) {
            cout << counter + 1 << ". " << category->getCategoryName() << endl;
            counter++;
        }
        cout << "Enter your choice: ";
        choice = Checker::selectAnswers(1,counter);
        choice--;
        cout<<endl;
        const vector<TestCategory *> categories = session.getCategoriesByIndex(choice);
        cout << "You selected category: " << categories[0]->getCategoryName() << endl; //
        counter = 0;
        const vector<Test *> testsInCategory = categories[0]->getTests();
        for (Test *findTest: testsInCategory) {
            cout << counter + 1 << ". " << findTest->getTestName() << endl;
            counter++;
        }
        cout << "Enter your choice: ";
        choice = Checker::selectAnswers(1,counter);
        choice--;
        cout<<endl;
        Test *selectedTest = testsInCategory[choice];
        cout << "You selected test: " << selectedTest->getTestName() << endl; //
        session.setTotalAmountOfQuestions(categoryName,testName);
        string unicalIdString = to_string(unicalId);
        for (TestCategory* category : session.getCategories()) {
            int coutCorrect = 0;
            if (category->getCategoryName() == categories[0]->getCategoryName()) {
                for (Test *test: category->getTests()) {
                    if (test->getTestName() == selectedTest->getTestName()) {
                        const vector<Question *> questionsInTest = test->getQuestions();
                        vector<string> testFiles = workWithFile.getTestFilesInFolder(pathToFolder);
                        string fileName ="Results/" + categories[0]->getCategoryName() + "_" + selectedTest->getTestName() + "_" + "unicalId_" + unicalIdString +".txt";
                        string fileNameForCheck = categories[0]->getCategoryName() + "_" + selectedTest->getTestName() + "_" + "unicalId_" + unicalIdString + ".txt";
                        int choose = 0;
                        for (const auto &fileNames: testFiles) {
                            if (fileNameForCheck == fileNames) {
                                int counter1 = 0;
                                int counter2 = 0;
                                bool testFound = false; // Флаг, чтобы отслеживать, был ли найден необходимый тест
                                for (Question *question : questionsInTest) {
                                    if (workWithFile.loadLastQuestionFromPausedTest(fileName) == question->getText()) {
                                        counter2++;
                                        testFound = true;
                                        break;
                                    }
                                    if (!testFound && counter2 == 0) {
                                        counter1++;
                                    }
                                }
                                if (counter1 + 1 < questionsInTest.size()) {
                                    cout << "1. Resume test\n";
                                    cout << "2. Start new test\n";
                                    cout << "3. Go back\n";
                                    cout << "Enter your choose: ";
                                    choose = Checker::selectAnswers(1, 3);
                                }
                                if (choose == 1) {
                                    restartOrContinue = true;
                                } else if (choose == 3) {
                                    return;
                                } else {
                                    restartOrContinue = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        student.startTest(session, this->unicalId, categories[0]->getCategoryName(), selectedTest->getTestName(), this->pathToFolder, restartOrContinue);
    }
    //Testing
    void workWithAddingTest() {
        if (logForReg.getLogInAsAdmin() == true && logForReg.getLogInAsStudent() == false) {
            bool flag = false;
            while (!flag) {
                cout << "===== Main test Menu =====\n";
                cout << "1. Add category\n";
                cout << "2. Add test\n";
                cout << "3. Add questions to the test\n";
                cout << "4. Back to Main Menu\n";
                cout << "=========================\n";
                cout << "Enter your choice: ";
                int choice = Checker::selectAnswers(1,4);
                switch (choice) {
                    case 1:
                        addCategory();
                        break;
                    case 2:
                        addTestToCategory();
                        break;
                    case 3:
                        addQuestionToTheTest();
                        break;
                    case 4:
                        flag = true;
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
    void addCategory() {
        if (logForReg.getLogInAsAdmin() == true && logForReg.getLogInAsStudent() == false) {
            cout << "Enter category name: ";
            getline(cin, categoryName);

            for (TestCategory *category: session.getCategories()) {
                if(categoryName == category->getCategoryName()){
                    cout<<"Test with this name already exist "<<endl;
                    return;
                }
            }
            session.addTestCategory(move(categoryName));
        }
        else {
            cerr << "You haven't access " << endl;
        }
    }
    void addTestToCategory() {
        if (logForReg.getLogInAsAdmin() == true && logForReg.getLogInAsStudent() == false) {
            int counter = 0;
            int choice;
            for (TestCategory* category : session.getCategories()) {
                cout << counter + 1 << ". " << category->getCategoryName() << endl;
                counter++;
            }
            cout << endl << "Enter your choice: ";
            choice = Checker::selectAnswers(1,counter);
            cout<<endl;

            const vector<TestCategory*> categories = session.getCategoriesByIndex(--choice);
            categoryName = categories[0]->getCategoryName();
            cout << "Enter test name for category " << categoryName << ": ";
            getline(cin, testName);
            if (!categories.empty()) {
                const vector<Test *> testsInCategory = categories[0]->getTests();
                for (Test *findTest: testsInCategory) {
                    if(testName == findTest->getTestName()){
                        cout<<"Test with this name already exist "<<endl;
                        return;
                    }
                }
                Test* test = new Test(testName);
                session.addTestToCategory( categoryName, test);
            }
            else {
                cerr << "Error: no information " << endl;
            }
        }
        else {
            cerr << "You haven't access " << endl;
        }
    }
    void addQuestionToTheTest() {
        if (logForReg.getLogInAsAdmin() == true && logForReg.getLogInAsStudent() == false) {
            int counter = 0;
            int choice;
            for (TestCategory* category : session.getCategories()) {
                cout << counter + 1 << ". " << category->getCategoryName() << endl;
                counter++;
            }
            cout << "Enter your choice: ";
            choice = Checker::selectAnswers(1,counter);
            cout<<endl;
            choice--;
            counter = 0;
            for (TestCategory* category : session.getCategories()) {
                if(counter == choice){
                    counter = 0;
                    cout << "You selected category: " << category->getCategoryName()<< endl;
                    const vector<Test*> testsInCategory = category->getTests();
                    for (Test* findTest : testsInCategory) {
                        cout << counter + 1 << ". " << findTest->getTestName() << endl;
                        counter++;
                    }
                    if (!testsInCategory.empty()) {
                        cout << "Enter your choice: ";
                        choice = Checker::selectAnswers(1,counter);
                        cout<<endl;
                        choice--;
                        counter = 0;
                        for (Test* findTest : testsInCategory) {
                            if(counter == choice){
                                string questionText;
                                cout << "Enter question text: ";
                                getline(cin, questionText);
                                Question* question = new Question(questionText);
                                question->setText(questionText);
                                int optionCount = 4;
                                for (int i = 1; i <= optionCount; ++i) {
                                    string optionText;
                                    cout << "Enter option " << i << ": ";
                                    getline(cin, optionText);
                                    question->addOption(i, optionText);
                                }
                                cout << "Enter the correct answer option number: ";
                                int correctAnswer = Checker::selectAnswers(1,4);
                                question->setCorrectAnswer(correctAnswer);
                                session.addQuestionToTest(question, findTest);
                                cout << "Question added to the test successfully." << endl;
                                return;
                            }
                            else {
                                counter++;
                            }
                        }
                    }
                    else{counter++;
                    }
                }
                counter++;
            }
        }
        else { cerr << "You haven't access " << endl;
        }
    }
    void editTests(){
        cout<<endl<<endl<<"Bura kimi geldinizse halaldir "<<endl;
        cout<<"hele hazir deyil"<<endl;
        bool flag = false;
        while (!flag) {
            cout << "===== Edit test Menu =====\n";
            cout << "1. Change category name\n";
            cout << "2. Change test name\n";
            cout << "3. Change questions in tests\n";
            cout << "4. Back to Main Menu\n";
            cout << "=========================\n";
            cout << "Enter your choice: ";
            int choice = Checker::selectAnswers(1,4);
            switch (choice) {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    flag = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    void showResultsToAdmin(){
        bool flag = false;
        while (!flag) {
            cout << "===== Show result Menu =====\n";
            cout << "1. Show results by category\n";
            cout << "2. Show results by tests\n";
            cout << "3. Show results by users\n";
            cout << "4. Back to Main Menu\n";
            cout << "=========================\n";
            cout << "Enter your choice: ";
            int choice = Checker::selectAnswers(1,4);
            switch (choice) {
                case 1:
                    showResultByCategory();
                    break;
                case 2:
                    showResultsByTests();
                    break;
                case 3:
                    showResultsByUsers();
                    break;
                case 4:
                    flag = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    void showResultsByUsers() {
        int unicalId;
        int counter = 0;
        vector<User> users = usersWork.getUsers();
        int i = 0;
        for(auto& user : users){
            if(user.getUsertype() != "admin"){
                cout<<i+1<<"." <<user.getFullName()<<endl;
                i++;
            }
        }
        cout<<endl<<"Select user: ";
        int select = Checker::selectAnswers(1,i);
        size_t indexToDisplay = select;
        if (indexToDisplay <= users.size()) {
            cout<<"User full name: " <<users[indexToDisplay].getFullName()<<endl<<endl;
            unicalId = users[indexToDisplay].getUnicalId();
        }
        administrator.showResultOfUser(session, usersWork, this->pathToFolder, unicalId);
    }
    void showResultsByTests(){
        int counter = 0;
        int choice;
        for (TestCategory* category : session.getCategories()) {
            cout << counter + 1 << ". " << category->getCategoryName() << endl;
            counter++;
        }
        cout << "Enter your choice: ";
        choice = Checker::selectAnswers(1,counter);
        cout<<endl;
        choice--;
        counter = 0;
        for (TestCategory* category : session.getCategories()) {
            if(counter == choice){
                counter = 0;
                cout << "You selected category: " << category->getCategoryName()<< endl;
                const vector<Test*> testsInCategory = category->getTests();
                for (Test* findTest : testsInCategory) {
                    cout << counter + 1 << ". " << findTest->getTestName() << endl;
                    counter++;
                }
                cout << "Enter your choice: ";
                choice = Checker::selectAnswers(1,counter);
                cout<<endl;
                choice--;
                counter = 0;
                for (Test* findTest : testsInCategory) {
                    if(counter == choice){
                        administrator.showResultsByTests(session, usersWork, this->pathToFolder,category->getCategoryName(), findTest->getTestName());
                        return;
                    }
                    else {
                        counter++;
                    }
                }
            }
            counter++;
        }
    }
    void showResultByCategory(){
        int counter = 0;
        int choice;
        for (TestCategory* category : session.getCategories()) {
            cout << counter + 1 << ". " << category->getCategoryName() << endl;
            counter++;
        }
        cout << "Enter your choice: ";
        choice = Checker::selectAnswers(1,counter);
        cout<<endl;
        choice--;
        counter = 0;
        for (TestCategory* category : session.getCategories()) {
            if(counter == choice) {
                counter = 0;
                cout << "You selected category: " << category->getCategoryName() << endl;
                administrator.showResultByCategory(session, usersWork, this->pathToFolder,category->getCategoryName());
            }
            counter++;
        }
    }

public:
    void runner() {
        workWithFile.loadUsersFromFile(usersWork, this->usersTxt);
        workWithFile.loadTests(session,this->testsTxt);
        bool flag = false;
        while (!flag) {
            displayMainMenu();
            int choice = Checker::selectAnswers(1,3);
            switch (choice) {
                case 1:
                    displaySignInMenu();
                    break;
                case 2:
                    displayLogInMenu();
                    break;
                case 3:
                    flag = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
        workWithFile.saveUsersToFile(usersWork, this->usersTxt);
        workWithFile.saveTests(session, this->testsTxt);
    }
};
int main() {
    ConsoleInterface consoleInterface;
    consoleInterface.runner();
}
