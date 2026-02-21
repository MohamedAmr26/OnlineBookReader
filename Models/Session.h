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
        const User* user;
        int id;
        int page;
    public:
        Session(int b, const User* u, int id, int p) : book_id(b), user(u), id(id), page(p) {}
        void set_id(int new_id) {
            id = new_id;
        }
        Session& operator=(const Session& other) {
            if (this != &other) {
                book_id = other.book_id;
                user = other.user;
                id = other.id;
                page = other.page;
            }
            return *this;
        }
        Session(const Session& other)
            : book_id(other.book_id), user(other.user), id(other.id), page(other.page) {}
        int get_id() const {
            return id;
        }
        int get_book_id() const {
            return book_id;
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