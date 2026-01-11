#include <iostream>
#include <vector>

using namespace std;

#ifndef SESSION_H
#define SESSION_H

class Session{
    private:
        string book_name;
        int page;
        string date;
    public:
        Session(string book_name, int p, string d) 
            : book_name(book_name), page(p), date(d) {}

        string get_book_name() const {
            return book_name;
        }

        int get_page() const {
            return page;
        }

        string get_date() const {
            return date;
        }

        void set_page(int p) {
            page = p;
        }

        void set_date(string d) {
            date = d;
        }
};

#endif