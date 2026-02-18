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
        Session(const Book* b, const User* u, int i, int p) : book(b), user(u), id(i), page(p) {}
        Session& operator=(const Session& other) {
            if (this != &other) {
                book = other.book;
                user = other.user;
                id = other.id;
            }
            return *this;
        }
        Session(const Session& other)
            : book(other.book), user(other.user), id(other.id) {}
        int get_id() const {
            return id;
        }
        const Book* get_book() const {
            return book;
        }
        const User* get_user() const {
            return user;
        }
};


#endif