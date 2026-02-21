#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <limits>


#include "./Models/User.h"
#include "./Models/Book.h"
#include "./Models/Session.h"
#include "UI.h"

using namespace std;

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
        cin.ignore();
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
    int AskBookSessions(const string& book_name, const vector<pair<int, string> >& session_details){
        cout << "\nReading Sessions for \"" << book_name << "\":\n";
        for (int i = 0; i < session_details.size(); ++i) {
            cout << (i+1) << "- " << session_details[i].second << endl;
        }

        int choice;
        cout << "0" << "- " << "Create new Session\n";
        cin >> choice;

        if (choice && choice <= session_details.size()) {
            return session_details[choice-1].first;
        }
        while (choice < 0 || choice > session_details.size()) {
            cout << "Invalid choice. Please enter a number between 0 and " << session_details.size() << ": ";
            cin.ignore();
            cin >> choice;
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
    int askUserSessions(const vector<pair<int, string> >& session_details){
        cout << "\nYour Reading Sessions:\n";

        for (int i = 0; i < session_details.size(); ++i) {
            cout << i+1 << " - " << session_details[i].second << endl;
        }
        cout << session_details.size()+1 <<"- Return to Main Menu\n";

        int c;
        cin >> c;
        if (c <= session_details.size()){
            return session_details[c-1].first;
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
        cin.ignore();
    }
    void askPageContent(int page_num, string &content){
        cout << "Enter content for page " << page_num << ": ";
        getline(cin, content);
    }
};

class DatastoreService {
    private:
        static map<string, pair<string, bool> > accounts;
    public:        
        static void load_accounts(const map<string, pair<string, bool> >& accs) {
            accounts = accs;
        }
        static map<string, pair<string, bool> >& get_accounts() {
            return accounts;
        }
        static void add_account(const string& username, const string& password, bool isAdmin) {
            accounts[username] = make_pair(password, isAdmin);
        }
        static pair<int, bool> get_account(const string& username, const string& password) {
            auto it = accounts.find(username);
            if (it != accounts.end() && it->second.first == password) {
                return make_pair(1, it->second.second);
            }
            return {-1, false};
        }
};
map<string, pair<string, bool> > DatastoreService::accounts;

class AuthenticationService {
    public:
        static User* login(string &username, string &password){
            auto result = DatastoreService::get_account(username, password);
            if (result.first == 1) {
                if (result.second) {
                    return new Admin(username, password);
                } else {
                    return new Customer(username, password);
                }
            }
            return nullptr;
        }
        static bool logout(User* user) {
            if (user)
                delete user;
            else
                return false;
            return true;
        }
};

class LibraryService {
    private:
        static vector<Book> Books;

    static const Book &get_book(int book_id){
        for (const auto &book : Books){
            if (book.get_id() == book_id)
            {
                return book;
            }
        }
        throw out_of_range("Book not found");
    }
    static void show_page(string page_content, int page_num){
        cout << "\n--- Page " << page_num+1 << " ---\n";
        cout << page_content << endl;
    }
    static void ask_controllers(const Book& book, int& page_num){
        cout << "1-Previous Page" << "            " << "2-Next Page" << "            " << "3-Save&Exit\n";
        int n;
        cin >> n;

        switch (n)
        {
        case 2:
            if (!book.is_out_borders(page_num + 1))
            {
                page_num++;
            }
            show_page(book.get_page(page_num), page_num);
            ask_controllers(book, page_num);
            break;
        case 1:
            if (!book.is_out_borders(page_num - 1))
            {
                page_num--;
            }
            show_page(book.get_page(page_num), page_num);
            ask_controllers(book, page_num);
            break;
        case 3:
            break;
        default:
            ask_controllers(book, page_num);
            break;
        }
    }
public:
    static const int add_book(const string& book_name, const string& author, int num_pages, vector<string> pages)
    {
        Book new_book(book_name, author, pages);
        if (find(Books.begin(), Books.end(), new_book) == Books.end())
        {
            Books.push_back(new_book);
            return new_book.get_id();
        }
        throw out_of_range("Book already exists");
    }
    static bool remove_book(int book_id){
        auto it = find_if(Books.begin(), Books.end(), [&book_id](const Book& book){
            return book.get_id() == book_id;
        });
        if (it != Books.end())
        {
            Books.erase(it);
            return true;
        }
        return false;
    }
    static int no_books()
    {
        return Books.size();
    }
    static vector<string> list_book_names()
    {
        vector<string> book_names;
        for (const auto &book : Books)
        {
            book_names.push_back(book.get_book_name());
        }
        return book_names;
    }
    static vector<int> list_books()
    {
        vector<int> book_refs;
        for (const auto &book : Books)
        {
            book_refs.push_back(book.get_id());
        }
        return book_refs;
    }
    static int openBook(int id, int page)
    {
        const Book& book = get_book(id);

        if (!book.is_out_borders(page)){
            show_page(book.get_page(page), page);
            ask_controllers(book, page);
            return page;
        }
        return -1;
    }
};
vector<Book> LibraryService::Books;

class SessionService {
    private:
        static vector<Session> Sessions;
        static int next_id;

        static Session& get_session(int session_id) {
            for (auto& session : Sessions) {
                if (session.get_id() == session_id) {
                    return session;
                }
            }
            throw out_of_range("Session not found");
        }
    public:
        static int add_session(const User& user, int book_id, int page) {
            Session session(book_id, &user, next_id++, page);
            Sessions.push_back(session);
            return Sessions.back().get_id();
        }
        static vector<pair<int, string>> list_sessions_for(const User& user, int book_id = -1) {
            vector<pair<int, string>> session_strings;
            for (const auto& session : Sessions) {
                if (user == *session.get_user() && (book_id == -1 || session.get_book_id() == book_id)) {
                    session_strings.push_back(make_pair(session.get_id(), session.toString()));
                }
            }
            return session_strings;
        }
        static void remove_sessions_for_book(int book_id) {
            Sessions.erase(remove_if(Sessions.begin(), Sessions.end(),
                [&book_id](const Session& session) {
                    return session.get_book_id() == book_id;
                }), Sessions.end());
        }
        // get bookid, pagenum from session by sessionid
        
};
vector<Session> SessionService::Sessions;
int SessionService::next_id = 1;

class App {
private:
    static UI* app_ui;

    static User* loginUser(){
        string username, password;
        app_ui->askLogin(username, password);

        User* user = AuthenticationService::login(username, password);

        while (!user)
        {
            app_ui->showInvalidLogin();
            app_ui->askLogin(username, password);
            user = AuthenticationService::login(username, password);
        }

        return user;
    }
    static void main_menu_non_admin(int current_choice, User* user){
        switch (current_choice){
            case 1:
                app_ui->displayProfile(user->get_username(), user->isAdmin(), SessionService::list_sessions_for(*user).size());
                loadMainMenu(user);
                break;
            case 2:
            {
                vector<string> books_list = LibraryService::list_book_names();
                app_ui->showBooks(books_list);

                if (books_list.empty()) {
                    cout << "No books available in the library.\n";
                    app_ui->askReturnToMainMenu();
                    loadMainMenu(user);
                    break;
                }
                
                int bookChoice = app_ui->askBookChoose(books_list.size());
                string bookName = books_list[bookChoice-1];
                const Book& book = LibraryService::get_book(bookName);

                auto sessions_list =  SessionService::list_sessions_for(*user, &book);
                
                int sessionID = app_ui->AskBookSessions(bookName, sessions_list);

                if (sessionID == -1){
                    Session &new_session = SessionService::add_session(*user, book, 0);

                    int newPage = LibraryService::openBook(*new_session.get_book(), new_session.get_page());
                    new_session.set_page(newPage);
                }else{
                    Session &book_session = SessionService::get_session(sessionID);
                    int newPage = LibraryService::openBook(*book_session.get_book(), book_session.get_page());
                    
                    book_session.set_page(newPage);
                }
                loadMainMenu(user);
                break;
            }
            case 3:
            {
                auto sessions_list =  SessionService::list_sessions_for(*user);
                int sessionID = app_ui->askUserSessions(sessions_list);

                if (sessionID != -1){
                    Session &book_session = SessionService::get_session(sessionID);
                    int newPage = LibraryService::openBook(*book_session.get_book(), book_session.get_page());
                    book_session.set_page(newPage);
                }
                loadMainMenu(user);
                break;
            }
            case 4:
                cout << "Logged out successfully.\n";
                if (!AuthenticationService::logout(user)) {
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
    static void main_menu_admin(int current_choice, User* user){
         switch (current_choice){
            case 1:
                app_ui->displayProfile(user->get_username(), user->isAdmin(), SessionService::list_sessions_for(*user).size());
                loadMainMenu(user);
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
                        app_ui->askBookDetails(book_name, author, num_pages);
                        vector<string> pages(num_pages);

                        for (int i = 0; i < num_pages; ++i) {
                            app_ui->askPageContent(i+1, pages[i]);
                        }

                        try{
                            LibraryService::add_book(book_name, author, num_pages, pages);
                            cout << "Book added successfully!\n";
                        }catch (const exception& e) {
                            cout << "Error adding book: " << e.what() << "\n";
                        }
                        loadMainMenu(user);
                        break;
                    }
                    case 2:
                    {
                        vector<string> books_list = LibraryService::list_book_names();
                        app_ui->showBooks(books_list);

                        int bookChoice = app_ui->askBookChoose(books_list.size());
                        const Book& book = LibraryService::get_book(books_list[bookChoice-1]);
                        SessionService::remove_sessions_for_book(book);
                        bool res = LibraryService::remove_book(book);

                        if (!res) {
                            cout << "Error removing book.\n";
                        } else {
                            cout << "Book removed successfully!\n";
                        }
                        loadMainMenu(user);
                        break;
                    }
                    case 3:
                    {
                        vector<string> books_list = LibraryService::list_book_names();
                        app_ui->showBooks(books_list);
                        app_ui->askReturnToMainMenu();
                        loadMainMenu(user);
                        break;
                    }
                    case 4:
                        loadMainMenu(user);
                        break;
                }                
                break;
            }
            case 3:
                cout << "Logged out successfully.\n";
                if (!AuthenticationService::logout(user)) {
                    cout << "Error logging out.\n";
                    return;
                }
                run();
            case 4:
                return;
         }
     }
    static void loadMainMenu(User* user){
        int current_choice = app_ui->askMainMenu(user->isAdmin());
        if (user->isAdmin()){
            main_menu_admin(current_choice, user);
        }else
            main_menu_non_admin(current_choice, user);
    }
public:
    static void setUI(UI* ui) {
        app_ui = ui;
    }
    static void run() {
        app_ui->showWelcome();
        User* user = loginUser();

        if (user == nullptr) {
            cout << "Login failed. Closing program.\n";
            return;
        }

        loadMainMenu(user);
    }
};

UI* App::app_ui = nullptr;

int main(){
    map<string, pair<string, bool> > accounts;
    accounts["mohamedAmr26"] = make_pair("examplePassword1", false);
    accounts["ahmedSasa57"] = make_pair("examplePassword2", false);
    accounts["seriousAdmin"] = make_pair("adminPassword1", true);
    DatastoreService::load_accounts(accounts);

    vector<string> pages(3);
    pages[0] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    pages[1] = "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).";
    pages[2] = "Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source.";
    LibraryService::add_book("Mohamed in Wonderland", "SussyBaka", 3, pages);

    App::setUI(new ConsoleUI());
    App::run();
}