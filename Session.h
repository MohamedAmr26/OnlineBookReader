#include <iostream>
#include <vector>

using namespace std;

#ifndef SESSION_H
#define SESSION_H

class Book;
class User;

class Session{
    private:
        const Book* book;
        const User* user;
        int id;
        int page;
    public:
        Session(const Book* b, const User* u, int id, int p) : book(b), user(u), id(id), page(p) {}
        void set_id(int new_id) {
            id = new_id;
        }
        Session& operator=(const Session& other) {
            if (this != &other) {
                book = other.book;
                user = other.user;
                id = other.id;
                page = other.page;
            }
            return *this;
        }
        Session(const Session& other)
            : book(other.book), user(other.user), id(other.id), page(other.page) {}
        int get_id() const {
            return id;
        }
        const Book* get_book() const {
            return book;
        }
        const User* get_user() const {
            return user;
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