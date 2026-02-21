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
    static int next_id;
    int id;
public:
    bool is_out_borders(int pos) const;
    Book(string name, string author, const vector<string> &pages)
        : name(name), author(author), pages(pages) {
            id = ++next_id;
        }
    void add_page(string page_context);
    void remove_page(int pos);
    string get_page(int num) const;
    int get_no_pages() const;
    string get_book_name() const;
    string get_book_author() const;
    int get_id() const;
    void change_book_name(string new_name);
    void change_book_author(string new_author);
    bool operator==(const Book &b) const
    {
        return this->get_id() == b.get_id();
    }
};

#endif