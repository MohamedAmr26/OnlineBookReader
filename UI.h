#include <iostream>
#include <vector>
using namespace std;

#ifndef UI_H
#define UI_H

class UI{
public:
    virtual void showWelcome() = 0;
    virtual void askLogin(string& username, string& password) = 0;
    virtual void showInvalidLogin() = 0;
    virtual int askMainMenu(bool isAdmin) = 0;
    virtual void showBooks(const vector<string>& books) = 0;
    virtual int askBookChoose(int size) = 0;
    virtual int AskBookSessions(const string& book_name, const vector<string>& session_strings, const vector<int>& session_ids) = 0;
    virtual void displayProfile(string username, bool isAdmin, int sessionCount) = 0;
    virtual int askUserSessions(const vector<string>& session_strings) = 0;
    virtual int askAdminLibraryMenu() = 0;
    virtual void askBookDetails(string& book_name, string& author, int& num_pages) = 0;
    virtual void askPageContent(int page_num, string& content) = 0;
    virtual void askReturnToMainMenu() = 0;
    virtual ~UI() = 0;
};
#endif