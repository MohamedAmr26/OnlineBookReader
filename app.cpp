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

    int showMainMenu(bool isAdmin) {
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
    int askBookSessions(int start){
        int choice = start;
        cout << start << "- " << "Create new Session\n";
        cin >> choice;
        return choice;
    }
};

int main(){
    map<string, pair<string, bool> > accounts;
    accounts["mohamedAmr26"] = make_pair("examplePassword1", false);
    accounts["ahmedSasa57"] = make_pair("examplePassword2", false);
    accounts["seriousAdmin"] = make_pair("adminPassword1", true);

    User::load_accounts(accounts);

    User my_user;
    Library library;
    ConsoleUI UI;

    vector<string> pages(1);
    pages[0] = "lorem ipsum";
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
    int current_choice = UI.showMainMenu(my_user.is_admin());

    if (!my_user.is_admin()){
        switch (current_choice){
            case 2:
                vector<string> books_list = library.list_books();
                UI.showBooks(books_list);

                int bookChoice = UI.askBookChoose(books_list.size());

                my_user.list_sessions();
                int choice = UI.askBookSessions(my_user.sessionsSize()+1);
                    
                    try{
                        Session book_session;
                        Book my_book = library.get_book(bookChoice-1);
                        
                        if (choice == my_user.sessionsSize() + 1) {
                            book_session = my_user.create_session(my_book.get_book_name(), 1);
                        } else {
                            book_session = my_user.get_session(choice-1);
                        }
                        
                        my_book.open(book_session);
                    }catch(out_of_range err){
                        cout << err.what() <<endl;
                        // should return asking again
                    }
                break;
            
        }
    }
}