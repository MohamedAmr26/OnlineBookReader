#include <iostream>
#include <map>
#include <vector>

using namespace std;

#ifndef USER_H
#define USER_H
class Session;

// thinking of using JSON later
class User{
    private:
        string username;
        string password;
        bool isAdmin;
        vector<Session> sessions;
        // private static?
        static map<string, pair<string, bool> > accounts;
    public:
        User(){
            username = "";
            password = "";
            isAdmin = false;
        }
        static void load_accounts(const map<string, pair<string, bool> >& accounts) {
            User::accounts = accounts;
        }            
        bool login(string in_username, string in_password){
            if (accounts.count(in_username) && accounts[in_username].first == in_password){
                username = in_username;
                password = in_password;
                isAdmin = accounts[in_username].second;
                return true;
            }
            return false;
        }
        bool is_admin() const{
            return isAdmin;
        }
        string get_username() const{
            return username;
        }
        static bool is_authorized(const User& user){
            return user.is_admin();
        }
        int sessionsSize() const{
            return sessions.size();
        }
        const vector<Session>& get_sessions() const;

        void insert_session(const Session& session);

        const Session& get_session(int pos) const{
            if (pos >= 0 && pos < sessionsSize()){
                return sessions[pos];
            }
            throw out_of_range("Invalid Session Number");
        }
};
#endif