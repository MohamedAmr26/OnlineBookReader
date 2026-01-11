#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "User.h"
#include "Book.h"
#include "Session.h"

using namespace std;

class Inventory{
    private:
        vector<Book> Books;
    public:
        Inventory(){
        }
        void insert_book(const Book& new_book, const User& user){
            if (!User::is_authorized(user)) return;
            auto it = find(Books.begin(), Books.end(), new_book);
            if (it == Books.end())
                Books.push_back(new_book);
            else
                cout << "Book "<< new_book.get_book_name() << " already exists" <<endl;
        }
        bool remove_book(const Book& new_book, const User& user){
            if (!User::is_authorized(user)) return false;
            auto it = find(Books.begin(), Books.end(), new_book);
            int idx = distance(Books.begin(), it);

            if (it != Books.end()){
                Books.erase(it);
                return true;
            }
            return false;
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

int main(){
    map<string, pair<string, bool> > accounts;
    accounts["mohamedAmr26"] = make_pair("examplePassword1", false);
    accounts["ahmedSasa57"] = make_pair("examplePassword2", false);
    accounts["seriousAdmin"] = make_pair("adminPassword1", true);

    User::load_accounts(accounts);

    string username;
    string password;

    cout << "Welcome to Online Book Reader!"<<endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    User my_user;

    while (!my_user.login(username, password)){
        cin.clear();
        cin.ignore(1000);
        cout << "Invalid username/password"<<endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
    }

    // main menu

    cout << "\n\n\n---- Main menu ----\n";
    cout << "1- View Profile\n";

    if (my_user.is_admin()){
        cout << "2- Modify Library\n";
        cout << "3- Logout\n";
    }else{
        cout << "2- Book List\n";
        cout << "3- My Reading Sessions\n";
        cout << "4- Logout\n";
    }
}