#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "User.h"
#include "Book.h"
#include "Session.h"
#include "library.h"

/*
Application
|-- UI (Interface)
    |-- ConsoleUI (Implementation)

Application
|-- Service
    |-- AuthenticationService
    |-- UserService

Service
|-- Models
    |-- User
    |-- Book
    |-- Session
    |-- Library
*/

using namespace std;
class UserService;

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

    void askReturnToMainMenu() {
        cout << "(0-9) Return to Main Menu\n";
        int c;
        cin >> c;
    }
    void displayProfile(string username, bool isAdmin, int sessionCount) {
        cout << "Username: " << username << "\n" << "Admin: " << isAdmin << "\n"
        << "Number of book sessions: " << sessionCount << endl;
        askReturnToMainMenu();
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
    int askAdminLibraryMenu(){
        int choice;
        cout << "\n---- Modify Library ----\n";
        cout << "1- Add Book\n";
        cout << "2- Remove Book\n";
        cout << "3- View Books\n";
        cout << "4- Return to Main Menu\n";
        cin >> choice;
        return choice;
    }
    void askBookDetails(string &book_name, string &author, int &num_pages){
        cout << "Enter book name: ";
        cin.ignore();
        getline(cin, book_name);
        cout << "Enter author name: ";
        getline(cin, author);
        cout << "Enter number of pages: ";
        cin >> num_pages;
    }
    void askPageContent(int page_num, string &content){
        cout << "Enter content for page " << page_num << ": ";
        cin.ignore();
        getline(cin, content);
    }
};

class AuthenticationService {
    private:
        static User* user;
    public:
        static bool login(string &username, string &password){
            return user->login(username, password);
        }
        static bool isAuthorized() {
            return user->is_admin();
        }
        static bool logout() {
            try{
                user->logout();
                return true;
            }catch(...){
                return false;
            }
        }
        friend class UserService;
};
User* AuthenticationService::user = new User(); // why cannot do it inside main?

class UserService {
    private:
        static User* user;
    public:
        static void syncUser() {
            user = AuthenticationService::user;
        }
        static string get_username() {
            return user->get_username();
        }
        static int sessionsSize() {
            return user->sessionsSize();
        }
        static const vector<Session>& get_user_book_sessions() {
            return user->get_sessions();
        }
        static void insert_session(const Session& session) {
            user->insert_session(session);
        }
        static Session& get_session(int id) {
            return user->get_session(id);
        }
        static vector<pair<int, string>> list_sessions() {
            vector<pair<int, string>> session_pairs;
            for (const auto& session : user->get_sessions()) {
                session_pairs.push_back({session.get_id(), session.toString()});
            }
            return session_pairs;
        }
};
User* UserService::user = nullptr;

class LibraryService {
    private:
        static Library* library;
    public:
        static void setLibrary(Library* lib) {
            library = lib;
        }
        static vector<string> list_books() {
            return library->list_books();
        }
        static void insert_book(const Book& book) {
            library->insert_book(book);
        }
        static string get_book_name(int pos) {
            return library->get_book(pos).get_book_name();
        }
        static void remove_book(int pos) {
            library->remove_book(pos);
        }
        static vector<string> get_sessions_for(string book_name) {
            vector<string> session_strings;
            for (const auto& session : UserService::get_user_book_sessions()) {
                if (session.get_book_name() == book_name) {
                    session_strings.push_back(session.toString());
                }
            }
            return session_strings;
        }
        static vector<int> get_session_ids_for(string book_name) {
            vector<int> session_ids;
            for (const auto& session : UserService::get_user_book_sessions()) {
                if (session.get_book_name() == book_name) {
                    session_ids.push_back(session.get_id());
                }
            }
            return session_ids;
        }
        static void openBook(string book_name, Session& session) {
            const Book& book = library->get_book(book_name);
            book.open(session);
        }
};
Library* LibraryService::library = nullptr;

class App {
private:
    static UI* app_ui;

    static void loginUser(){
        string username, password;
        app_ui->askLogin(username, password);

        while (!AuthenticationService::login(username, password))
        {
            app_ui->showInvalidLogin();
            app_ui->askLogin(username, password);
        }

        UserService::syncUser();
    }
    static void main_menu_non_admin(int current_choice){
        switch (current_choice){
            case 1:
                app_ui->displayProfile(UserService::get_username(), false, UserService::sessionsSize());
                loadMainMenu();
                break;
            case 2:
            {
                vector<string> books_list = LibraryService::list_books();
                app_ui->showBooks(books_list);

                int bookChoice = app_ui->askBookChoose(books_list.size());
                string bookName = books_list[bookChoice-1];

                int sessionID = app_ui->AskBookSessions(bookName, LibraryService::get_sessions_for(bookName), LibraryService::get_session_ids_for(bookName));

                if (sessionID == -1){
                    Session new_session(bookName, 0, "20 Jan 2026");
                    UserService::insert_session(new_session);

                    Session &book_session = UserService::get_session(new_session.get_id());
                    LibraryService::openBook(bookName, book_session);
                }else{
                    Session &book_session = UserService::get_session(sessionID);
                    LibraryService::openBook(bookName, book_session);
                }
                loadMainMenu();
                break;
            }
            case 3:
            {
                vector<pair<int, string>> session_strings = UserService::list_sessions();
                vector<string> session_string_values;
                vector<int> session_ids;
                for (const auto& pair : session_strings) {
                    session_string_values.push_back(pair.second);
                    session_ids.push_back(pair.first);
                }
                int sessionIndex = app_ui->askUserSessions(session_string_values);

                if (sessionIndex != -1){
                    Session &book_session = UserService::get_session(session_ids[sessionIndex]);
                    LibraryService::openBook(book_session.get_book_name(), book_session);
                }
                loadMainMenu();
                break;
            }
            case 4:
                cout << "Logged out successfully.\n";
                if (!AuthenticationService::logout()) {
                    cout << "Error logging out.\n";
                    return;
                }
                run();
                break;
            case 5:
                return;
            default:
                break;
            }
    }
    static void main_menu_admin(int current_choice){
         switch (current_choice){
            case 1:
                app_ui->displayProfile(UserService::get_username(), true, UserService::sessionsSize());
                loadMainMenu();
                break;
            case 2:
            {
                // Modify Library
                int adminChoice = app_ui->askAdminLibraryMenu();
                switch (adminChoice){
                    case 1:
                    {
                        string book_name, author;
                        int num_pages;
                        vector<string> pages(num_pages);
                        app_ui->askBookDetails(book_name, author, num_pages);
                        for (int i = 0; i < num_pages; ++i) {
                            app_ui->askPageContent(i+1, pages[i]);
                        }
                        Book new_book(book_name, author, pages);
                        LibraryService::insert_book(new_book);
                        cout << "Book added successfully!\n";
                        loadMainMenu();
                        break;
                    }
                    case 2:
                    {
                        vector<string> books_list = LibraryService::list_books();
                        app_ui->showBooks(books_list);
                        int bookChoice = app_ui->askBookChoose(books_list.size());
                        LibraryService::remove_book(bookChoice-1);
                        cout << "Book removed successfully!\n";
                        loadMainMenu();
                        break;
                    }
                    case 3:
                    {
                        vector<string> books_list = LibraryService::list_books();
                        app_ui->showBooks(books_list);
                        app_ui->askReturnToMainMenu();
                        break;
                    }
                    case 4:
                        loadMainMenu();
                        break;
                }                
                break;
            }
            case 3:
                cout << "Logged out successfully.\n";
                if (!AuthenticationService::logout()) {
                    cout << "Error logging out.\n";
                    return;
                }
                run();
            case 4:
                return;
         }
     }
   static void loadMainMenu(){
        int current_choice = app_ui->askMainMenu(AuthenticationService::isAuthorized());
        if (AuthenticationService::isAuthorized()){
            main_menu_admin(current_choice);
        }else
            main_menu_non_admin(current_choice);
    }
public:
    static void setUI(UI* ui) {
        app_ui = ui;
    }
    static void run() {
        app_ui->showWelcome();
        loginUser();
        loadMainMenu();
    }
};

UI* App::app_ui = nullptr;

int main(){
    map<string, pair<string, bool> > accounts;
    accounts["mohamedAmr26"] = make_pair("examplePassword1", false);
    accounts["ahmedSasa57"] = make_pair("examplePassword2", false);
    accounts["seriousAdmin"] = make_pair("adminPassword1", true);

    User::load_accounts(accounts);

    Library library;
    LibraryService::setLibrary(&library);

    vector<string> pages(3);
    pages[0] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    pages[1] = "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).";
    pages[2] = "Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source.";
    Book mohamed_iwl("Mohamed in Wonderland", "SussyBaka", pages);
    LibraryService::insert_book(mohamed_iwl);

    App::setUI(new ConsoleUI());
    App::run();
}