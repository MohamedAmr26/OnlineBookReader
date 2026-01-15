#include "User.h"
#include "Session.h"

// static member definition
map<string, pair<string, bool> > User::accounts;

const vector<Session>& User::get_sessions() const{
    return sessions;
}

void User::insert_session(const Session& new_session){
    sessions.push_back(new_session);
}