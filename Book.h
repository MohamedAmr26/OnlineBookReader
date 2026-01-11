#include <iostream>
#include <vector>

using namespace std;

#ifndef BOOK_H
#define BOOK_H
class User;

class Book{
private:
    string name;
    string author;
    vector<string> pages;
public:
    Book(string name, string author, const vector<string> &pages)
        : name(name), author(author), pages(pages) {}
    void add_page(string page_context, const User &user);
    void remove_page(int pos, const User &user);
    string get_page(int num) const;
    int get_no_pages() const;
    string get_book_name() const;
    string get_book_author() const;
    void change_book_name(string new_name, const User &user);
    void change_book_author(string new_author, const User &user);
    bool operator==(const Book &b)
    {
        return this->get_book_name() == b.get_book_name();
    }
};

#endif