#include <iostream>
#include <vector>

using namespace std;

#ifndef BOOK_H
#define BOOK_H
class User;
class Session;
class Book{
private:
    string name;
    string author;
    vector<string> pages;
    int current_page=0;

    void show_page(int p);
    void ask_controllers();
    bool is_out_borders(int pos) const;
public:
    Book(string name, string author, const vector<string> &pages)
        : name(name), author(author), pages(pages) {}
    void add_page(string page_context);
    void remove_page(int pos);
    string get_page(int num) const;
    int get_no_pages() const;
    string get_book_name() const;
    string get_book_author() const;
    void change_book_name(string new_name);
    void change_book_author(string new_author);
    bool operator==(const Book &b)
    {
        return this->get_book_name() == b.get_book_name();
    }
    void open(Session& session);
};

#endif