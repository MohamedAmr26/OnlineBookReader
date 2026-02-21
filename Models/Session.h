#include <iostream>
#include <vector>

using namespace std;

#ifndef SESSION_H
#define SESSION_H

class Book;
class User;

class Session{
    private:
        int book_id;
        string username;
        int id;
        int page;
    public:
        Session(int b, const string& u, int id, int p) : book_id(b), username(u), id(id), page(p) {}
        void set_id(int new_id) {
            id = new_id;
        }
        Session& operator=(const Session& other) {
            if (this != &other) {
                book_id = other.book_id;
                username = other.username;
                id = other.id;
                page = other.page;
            }
            return *this;
        }
        Session(const Session& other)
            : book_id(other.book_id), username(other.username), id(other.id), page(other.page) {}
        int get_id() const {
            return id;
        }
        int get_book_id() const {
            return book_id;
        }
        const string& get_username() const {
            return username;
        }
        int get_page() const {
            return page;
        }
        void set_page(int new_page) {
            page = new_page;
        }
        string toString() const;
};


#endif