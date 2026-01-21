#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "User.h"
#include "Book.h"
#include "Session.h"

using namespace std;

class Library{
    private:
    // change to set later
        vector<Book> Books;
    public:
        Library(){}
        bool insert_book(const Book& new_book){
            auto it = find(Books.begin(), Books.end(), new_book);
            if (it == Books.end()){
                Books.push_back(new_book);
                return true;
            }
            return false;
        }
        bool remove_book(int pos){
            if (pos >= 0 && pos < no_books()){
                Books.erase(Books.begin()+pos);
                return true;
            }
            return false;
        }
        const Book& get_book(int pos) const{
            if (pos >= 0 && pos < no_books())
                return Books[pos];
            throw out_of_range("Book index out of range");
        }
        int no_books() const{
            return Books.size();
        }
        vector<string> list_books() const{
            vector<string> book_names;
            for (const auto& book : Books) {
                book_names.push_back(book.get_book_name());
            }
            return book_names;
        }
};
class ConsoleUI {
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
        } else {
            cout << "2- Book List\n";
            cout << "3- My Reading Sessions\n";
            cout << "4- Logout\n";
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
    int AskBookSessions(const string& book_name, User& user) {
        cout << "\nReading Sessions for \"" << book_name << "\":\n";
        vector<const Session*> sessions = user.get_sessions_for(book_name);
        for (int i = 0; i < sessions.size(); ++i) {
            cout << (i + 1) << "- " << sessions[i]->toString() << endl;
        }

        int choice;
        cout << sessions.size()+1 << "- " << "Create new Session\n";
        cin >> choice;
        if (choice <= sessions.size()){
            return sessions[choice-1]->get_id();
        }
        return -1;
    }

    void displayProfile(const User& user){
        cout << "Username: " << user.get_username() << "\n" << "Admin: " << user.is_admin() << "\n"
        << "Number of book sessions: " << user.sessionsSize() << endl;
        cout << "(1-9)- Return to Main Menu\n";
        int c;
        cin >> c;
    }
};

User my_user;
Library library;
ConsoleUI UI;

void main_menu_runner(){
    int current_choice = UI.askMainMenu(my_user.is_admin());

    if (!my_user.is_admin()){
        switch (current_choice){
            case 1:
                UI.displayProfile(my_user);
                main_menu_runner();
                break;
            case 2:
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
                main_menu_runner();
                break;
        }
    }
}

int main(){
    map<string, pair<string, bool> > accounts;
    accounts["mohamedAmr26"] = make_pair("examplePassword1", false);
    accounts["ahmedSasa57"] = make_pair("examplePassword2", false);
    accounts["seriousAdmin"] = make_pair("adminPassword1", true);

    User::load_accounts(accounts);

    vector<string> pages(3);
    pages[0] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    pages[1] = "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).";
    pages[2] = "Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source.";
    Book mohamed_iwl("Mohamed in Wonderland", "SussyBaka", pages);
    library.insert_book(mohamed_iwl);

    // login
    string username;
    string password;

    UI.showWelcome();
    UI.askLogin(username, password);

    while (!my_user.login(username, password)){
        UI.showInvalidLogin();
        UI.askLogin(username, password);
    }

    // main menu
    main_menu_runner();
}