#include <iostream>
#include <map>
#include <vector>

using namespace std;

#ifndef USER_H
#define USER_H

class User{
    private:
        string username;
        string password;
    public:
        virtual string get_username() const{
            return username;
        }
        virtual void set_username(string username){
            this->username = username;
        }
        virtual string get_password() const{
            return password;
        }
        virtual void set_password(string password){
            this->password = password;
        }
        virtual bool operator==(const User& other) const{
            return username == other.username && password == other.password && isAdmin() == other.isAdmin();
        }
        virtual bool isAdmin() const = 0;
        virtual ~User() = default;
};
class Admin : public User {
    public:
        Admin(string username, string password) {
            set_username(username);
            set_password(password);
        }
        bool isAdmin() const override {
            return true;
        }
};
class Customer : public User {
    public:
        Customer(string username, string password) {
            set_username(username);
            set_password(password);
        }
        bool isAdmin() const override {
            return false;
        }
};
#endif