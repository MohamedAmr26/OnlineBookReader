#include "User.h"
#include <algorithm>
#include "Session.h"

// static member definition
map<string, pair<string, bool> > User::accounts;

const vector<Session>& User::get_sessions() const{
    return sessions;
}

void User::insert_session(const Session& new_session){
    sessions.push_back(new_session);
}

vector<const Session*> User::get_sessions_for(string book_name) const{
    vector<const Session*> result;
    for(const auto& session : sessions){
        if(session.get_book_name() == book_name){
            result.push_back(&session);
        }
    }
    return result;
}

Session& User::get_session(int id){
    auto it = find_if(sessions.begin(), sessions.end(), 
    [id](const Session& session) { return session.get_id() == id;});
    return *it;
}