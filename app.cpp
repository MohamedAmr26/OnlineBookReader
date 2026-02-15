#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "User.h"
#include "Book.h"
#include "Session.h"
#include "library.h"

using namespace std;

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
};

class ConsoleUI : public UI {
public:
    void showWelcome() {
        cout << "Welcome to Online Book Reader!" << endl;
    }

    void askLogin(string& username, string& password) {
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
    }

    void showInvalidLogin() {
        cout << "Invalid username/password" << endl;
    }

    int askMainMenu(bool isAdmin) {
        int choice;

        cout << "\n---- Main Menu ----\n";
        cout << "1- View Profile\n";

        if (isAdmin) {
            cout << "2- Modify Library\n";
            cout << "3- Logout\n";
            cout << "4- Exit\n";
        } else {
            cout << "2- Book List\n";
            cout << "3- My Reading Sessions\n";
            cout << "4- Logout\n";
            cout << "5- Exit\n";
        }

        cin >> choice;
        return choice;
    }

    void showBooks(const vector<string>& books) {
        cout << "\nBook List:\n";
        int i = 1;
        for (const auto& book : books) {
            cout << i++ << "- " << book << endl;
        }
    }
    int askBookChoose(int size){
        int choice;
        cin >> choice;
        while (choice < 1 || choice > size) {
            cout << "Invalid choice. Please enter a number between 1 and " << size << ": ";
            cin >> choice;
        }
        return choice;
    }
    int AskBookSessions(const string& book_name, const vector<string>& session_strings, const vector<int>& session_ids){
        cout << "\nReading Sessions for \"" << book_name << "\":\n";
        for (int i = 0; i < session_strings.size(); ++i) {
            cout << (i + 1) << "- " << session_strings[i] << endl;
        }

        int choice;
        cout << session_strings.size()+1 << "- " << "Create new Session\n";
        cin >> choice;
        if (choice <= session_strings.size()){
            return session_ids[choice-1]; // Return session ID of selected session
        }
        return -1;
    }

    void displayProfile(string username, bool isAdmin, int sessionCount) {
        cout << "Username: " << username << "\n" << "Admin: " << isAdmin << "\n"
        << "Number of book sessions: " << sessionCount << endl;
        cout << "(0-9)- Return to Main Menu\n";
        int c;
        cin >> c;
    }
    int askUserSessions(const vector<string>& session_strings){
        cout << "\nYour Reading Sessions:\n";

        for (int i = 0; i < session_strings.size(); ++i) {
            cout << i+1 << " - " << session_strings[i] << endl;
        }
        cout << session_strings.size()+1 <<"- Return to Main Menu\n";

        int c;
        cin >> c;
        if (c <= session_strings.size()){
            return c-1; // Return index of selected session
        }
        return -1;
    }
};

class AuthenticationService {
    private:
        static User* user;
    public:
        static bool login(string &username, string &password){
            return user->login(username, password);
        }
        static void setUser(User* u) {
            user = u;
        }
};

User* AuthenticationService::user = new User(); // why cannot do it inside main?

void main_menu_runner(UI* ui, Library& library){
    int current_choice = ui->askMainMenu(my_user.is_admin());

    if (!my_user.is_admin()){
        switch (current_choice){
            case 1:
                UI.displayProfile(my_user);
                main_menu_runner();
                break;
            case 2:
                if (true)
                {
                vector<string> books_list = library.list_books();
                UI.showBooks(books_list);

                int bookChoice = UI.askBookChoose(books_list.size());
                const Book& my_book = library.get_book(bookChoice-1);

                 int sessionID = UI.AskBookSessions(my_book.get_book_name(), my_user);
                    
                if (sessionID == -1) {
                    Session new_session(my_book.get_book_name(), 0, "20 Jan 2026");
                    my_user.insert_session(new_session);

                    Session& book_session = my_user.get_session(new_session.get_id());
                    my_book.open(book_session);
                } else {
                    Session& book_session = my_user.get_session(sessionID);
                    my_book.open(book_session);
                }
                }
                main_menu_runner();
                break;
            case 3:
                if (true)
                {
                int sessionID = UI.askUserSessions(my_user);
                if (sessionID != -1) {
                    Session& book_session = my_user.get_session(sessionID);
                    const Book& my_book = library.get_book(book_session.get_book_name());
                    my_book.open(book_session);
                }
                }
                main_menu_runner();
                break;
            case 4:
                cout << "Logged out successfully.\n";
                my_user.logout();
                loginUser();
                main_menu_runner();
                break;
            case 5:
                return;
            default:
                break;
        }
    }else{
        switch (current_choice){
            case 1:
                UI.displayProfile(my_user);
                main_menu_runner();
                break;
            case 2:
                // Library modification menu can be implemented here
                main_menu_runner();
                break;
            case 3:
                cout << "Logged out successfully.\n";
                my_user.logout();
                loginUser();
                main_menu_runner();
                break;
            case 4:
                return;
            default:
                break;
        }
    }
}

void loginUser(UI* ui){
    string username, password;
    ui->askLogin(username, password);

    while (!AuthenticationService::login(username, password)){
        ui->showInvalidLogin();
        ui->askLogin(username, password);
    }
}

int main(){
    map<string, pair<string, bool> > accounts;
    accounts["mohamedAmr26"] = make_pair("examplePassword1", false);
    accounts["ahmedSasa57"] = make_pair("examplePassword2", false);
    accounts["seriousAdmin"] = make_pair("adminPassword1", true);

    User::load_accounts(accounts);

    Library library;
    UI* app_ui = new ConsoleUI();

    vector<string> pages(3);
    pages[0] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    pages[1] = "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).";
    pages[2] = "Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source.";
    Book mohamed_iwl("Mohamed in Wonderland", "SussyBaka", pages);
    library.insert_book(mohamed_iwl);

    loginUser(app_ui);
    main_menu_runner(app_ui, library);
}