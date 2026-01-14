#include "User.h"
#include "Session.h"

// static member definition
map<string, pair<string, bool> > User::accounts;

vector<string> User::list_sessions() const{
    vector<string> session_list;
    int session_num = 1;
    for (const auto& session : sessions) {
        string session_str = to_string(session_num) + "- " + session.get_book_name() 
                           + " - " + session.get_date() 
                           + " - Page:" + to_string(session.get_page());
        session_list.push_back(session_str);
        session_num++;
    }
    return session_list;
}

void User::create_session(string bookName, int page){
    sessions.push_back(Session(sessions, page, "14 Jan 2026"));
}