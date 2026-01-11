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
        void insert_book(const Book& new_book){
            auto it = find(Books.begin(), Books.end(), new_book);
            if (it == Books.end())
                Books.push_back(new_book);
            else
                cout << "Book "<< new_book.get_book_name() << " already exists" <<endl;
        }
        bool remove_book(const Book& new_book){
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
    Inventory library;

    vector<string> pages(1);
    pages[0] = "lorem ipsum";
    Book mohamed_iwl("Mohamed in Wonderland", "SussyBaka", pages);
    library.insert_book(mohamed_iwl);

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
    int current_choice = 0;
    int max_choice = 0;

    cout << "\n\n\n---- Main menu ----\n";
    cout << "1- View Profile\n";

    if (my_user.is_admin()){
        max_choice = 3;
        cout << "2- Modify Library\n";
        cout << "3- Logout\n";
    }else{
        max_choice = 4;
        cout << "2- Book List\n";
        cout << "3- My Reading Sessions\n";
        cout << "4- Logout\n";
    }

    cin >>current_choice;

    while (current_choice > max_choice || cin.fail()){
        cin.clear();
        cin.ignore(1000);
        if (my_user.is_admin()){
            max_choice = 3;
            cout << "2- Modify Library\n";
            cout << "3- Logout\n";
        }else{
            max_choice = 4;
            cout << "2- Book List\n";
            cout << "3- My Reading Sessions\n";
            cout << "4- Logout\n";
        }
    }

    if (!my_user.is_admin()){
        switch (current_choice){
            case 2:
                cout << "Book List:\n";
                int book_num = 1;
                for (const auto& book : library.list_books()) {
                    cout << book_num << "- " << book << endl;
                    book_num++;
                }
                break;
        }
    }
}